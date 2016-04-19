#include "server.h"
#include "http-request.h"
#include "http-response.h"
#include <sys/types.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>
#include <thread>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

Server::Server(const char* host, const char* port)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // set up socket calls
    status = getaddrinfo(host, port, &hints, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    // find socket to bind to
    auto i = res;
    int yes = 1;
    for (; i != NULL; i = i->ai_next)
    {
        m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
        if (m_sockfd == -1) 
        {
            continue;
        }

        status = setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (status == -1)
        {
            continue;
        }

        status = bind(m_sockfd, res->ai_addr, res->ai_addrlen);
        if (status == -1)
        {
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    // check if reached end of linked list
    if (i == NULL)
    {
        cout << "could not bind to any sockets" << endl;
        exit(1);
    }

    // start listening
    status = listen(m_sockfd, 10);
    process_error(status, "listen");
}

void Server::accept_connections()
{
    while (1)
    {
        struct sockaddr_storage accepted_addr;
        socklen_t addr_size = sizeof(accepted_addr);
        int new_fd = accept(m_sockfd, (struct sockaddr *) &accepted_addr, &addr_size);
        if (new_fd < 0)
        {
            perror("accept");
            exit(1);
        }

        // spawn new thread for incoming connection
        if (new_fd > 0)
        {
            thread{process_request, new_fd}.detach();
        }
    }
}

void Server::process_request(int socket_fd)
{
    size_t buf_pos = 0;
    string data;
    data.resize(512);

    while (1)
    {
        // receive request
        int n_bytes = recv(socket_fd, &data[buf_pos], data.size() - buf_pos, 0);
        process_error(n_bytes, "recv");
        if (n_bytes == 0)
        {
            // Client closed
            close(socket_fd);
            return;
        }
        buf_pos += n_bytes;

        // if data buffer is full
        if (data.size() == buf_pos)
        {
            data.resize(512 + data.size());
        }

        // check if full request sent
        size_t req_end_pos = data.find("\r\n\r\n");
        if (req_end_pos != string::npos)
        {
            // process request
            bool keep_alive = false;
            string wire(data, 0, req_end_pos + 4);
            data = string(data, req_end_pos + 4, buf_pos - (req_end_pos + 4));
            buf_pos = data.length();
            data.resize(512);

            HttpRequest req;
            req.consume(wire);
            if (req.get_method() != "GET")
            {
                send_405_resp(socket_fd);
                return;
            }

            // check for version 1.1
            if (req.get_version() == "1.1")
            {
                keep_alive = true;
            }

            // but if 1.1 and Connection: close, set back
            if (req.get_header("Connection") == "Close")
            {
                keep_alive = false;
            }

            // tries to open file
            string path = "." + req.get_url();
            int file_fd = open(path.c_str(), O_RDONLY);
            if (file_fd < 0)
            {
                send_404_resp(socket_fd);
                return;
            }

            // check if regular file
            struct stat buf;
            int status = fstat(file_fd, &buf);
            if (status < 0 || !S_ISREG(buf.st_mode))
            {
                perror("fstat");
                send_404_resp(socket_fd);
                close(file_fd);
                return;
            }

            // send status 200
            send_200_resp(socket_fd, keep_alive, buf);

            // try to send file
            off_t pos = 0;
            do 
            {
                if (sendfile(socket_fd, file_fd, &pos, buf.st_size - pos) < 0)
                {
                    close(file_fd);
                    close(socket_fd);
                    return;
                }
            }
            while (pos != buf.st_size);
            

            close(file_fd);
            if (keep_alive == false)
            {
                close(socket_fd);
                return;
            }
        }
    }
}

void Server::send_200_resp(int fd, bool keep_alive, struct stat buf)
{
    HttpResponse resp;
    resp.set_status_code("200");
    resp.set_status_message("OK");
    resp.set_connection(keep_alive ? "Keep Alive" : "Close");
    resp.set_content_length(to_string(buf.st_size));
    string response = resp.encode();
    int status = send(fd, response.c_str(), response.size(), 0);
    process_error(status, "send");
}

void Server::send_404_resp(int fd)
{
    HttpResponse resp;
    resp.set_status_code("404");
    resp.set_status_message("Not Found");
    resp.set_connection("Close");
    string error = resp.encode();
    int status = send(fd, error.c_str(), error.size(), 0);
    process_error(status, "send");
    close(fd);
}

void Server::send_405_resp(int fd)
{
    HttpResponse resp;
    resp.set_status_code("405");
    resp.set_status_message("Method Not Allowed");
    resp.set_connection("Close");
    string error = resp.encode();
    int status = send(fd, error.c_str(), error.size(), 0);
    process_error(status, "send");
    close(fd);
}

void Server::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(&function[0]);
        exit(1);
    }
}