#include "client.h"
#include "http-request.h"
#include "http-response.h"
#include <string>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <fstream>

using namespace std;

Client::Client(map<pair<string, string>, vector<string> > urls, int n_urls)
{
    for (auto curr_host : urls)
    {
        const char* host = curr_host.first.first.c_str();
        const char* port = curr_host.first.second.c_str();

        connect_to_socket(host, port);

        for (size_t i = 0; i < curr_host.second.size(); i++)
        {
            HttpRequest req;
            req.set_method("GET");
            req.set_url(curr_host.second[i]);

            if (n_urls > 1)
            {
                req.set_version("1.1");
            }
            else 
            {
                req.set_version("1.0");
            }

            req.set_header("Host", host);

            // TODO: take care of partial send() and send failures
            string request = req.encode();
            send(m_sockfd, request.c_str(), request.size(), 0);

            string version = process_response(get_file_name(curr_host.second[i]));

            // If server returns a response with version 1.0 the client
            // must close the connection and create a new one
            if (version == "1.0")
            {
                close(m_sockfd);
                connect_to_socket(host, port);
            }
        }

        close(m_sockfd);
    }
}

string Client::get_file_name(string file_path)
{
    int last_slash = file_path.find_last_of("/\\");
    return file_path.substr(last_slash + 1);
}

void Client::connect_to_socket(const char* host, const char* port)
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

    // TODO: test socket timeout
    // find socket to bind to
    auto i = res;
    for (; i != NULL; i = i->ai_next)
    {
        m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
        if (m_sockfd == -1)
        {
            continue;
        }

        struct timeval timeout;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        // status = setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        // if (status == -1)
        // {
        //     continue;
        // }

        status = connect(m_sockfd, res->ai_addr, res->ai_addrlen);
        if (status == -1)
        {
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    // check if reached end of linked list
    // means could not find a socket to bind to
    if (i == NULL)
    {
        perror("bind to a socket");
        exit(1);
    }
}

string Client::process_response(string file_name)
{
    size_t buf_pos = 0;
    string data;
    string version = "1.0";
    data.resize(512);
    size_t content_length = 0;

    while (1)
    {
        // TODO: client should receive persistent responses
        int n_bytes = recv(m_sockfd, &data[buf_pos], data.size() - buf_pos, 0);
        process_error(n_bytes, "recv");

        if (n_bytes == 0)
        {
            // TODO: make sure 1.0 doesn't close connection, buffer data, and then send 0 for recv
            // Server closed connection
            return version;
        }
        buf_pos += n_bytes;

        // if finished receiving file
        if (data.size() == content_length)
        {
            ofstream output(file_name);
            output << data;
            output.close();

            data = string(data, content_length, buf_pos - content_length);
            return version;
        }
        // if data needs to be resized
        else if (data.size() == buf_pos)
        {
            data.resize(512 + data.size());
        }

        // check if full response sent
        size_t req_end_pos = data.find("\r\n\r\n");
        if (req_end_pos != string::npos)
        {
            string wire(data, 0, req_end_pos + 4);
            data = string(data, req_end_pos + 4, buf_pos - (req_end_pos + 4));
            buf_pos = data.length();

            HttpResponse resp;
            resp.consume(wire);

            // if not 404 error
            if (resp.get_status_code() == "200")
            {
                // use content length to reize data
                content_length = size_t(stoll(resp.get_content_length()));
                data.resize(content_length);

                // TODO: handle downgrade of version
                version = resp.get_version();
            }
        }
    }

    return version;
}

void Client::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(function.c_str());
        exit(1);
    }
}
