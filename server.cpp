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

bool Server::accept_connections()
{
    while (1)
    {
        struct sockaddr_storage accepted_addr;
        socklen_t addr_size = sizeof(accepted_addr);
        int new_fd = accept(m_sockfd, (struct sockaddr *) &accepted_addr, &addr_size);
        if (new_fd > 0)
        {
            thread{process_request, new_fd}.detach();
        }
    }
    return true;
}

void Server::process_request(int fd)
{
    cout << "accepted" << endl;
    size_t pos = 0;
    string data;
    data.resize(512);

    while (1)
    {
        //int n_bytes = recv(fd, &data[pos], data.size() - pos, 0);

        //size_t req_end_pos = data.find("\r\n\r\n");
        if (1)//if (req_end_pos != string::npos)
        {
            // Found the end of a request

            // Deal with HTTP request

            //string wire(data, 0, req_end_pos + 4);
            //HttpRequest req;
            //req.consume(wire);

            HttpResponse resp;

            //data = string(data, req_end_pos + 4, string::npos);

            struct stat buf;
            string path = "./Makefile";
            int file = open(path.c_str(), O_RDONLY);
            if (file < 0)
            {
                // 
            }
            int status = fstat(file, &buf);
            if (status < 0)
            {
                // 404 error
            }
            if (!S_ISREG(buf.st_mode))
            {
                // 404 error
            }
            off_t pos = 0;
            do 
            {
                if (sendfile(fd, file, &pos, buf.st_size - pos) < 0)
                {
                    exit(1);
                }
            }
            while (pos != buf.st_size);
        }

        if (data.size() == pos)
        {
            data.resize(2 * data.size());
        }
        close(fd);
    }
}

void Server::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(&function[0]);
        exit(1);
    }
}