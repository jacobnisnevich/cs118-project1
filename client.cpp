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

using namespace std;

Client::Client(map<pair<string, string>, vector<string> > urls, int n_urls)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;



    for (auto curr_host : urls)
    {
        const char* host = curr_host.first.first.c_str();
        const char* port = curr_host.first.second.c_str();

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

            // TODO: take care of partial send() and send failures
            string request = req.encode();
            send(m_sockfd, request.c_str(), request.size(), 0);

            process_response();
        }

        close(m_sockfd);
    }
}

void Client::process_response()
{
    size_t buf_pos = 0;
    string data;
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
            return;
        }
        buf_pos += n_bytes;

        // if finished receiving file
        if (data.size() == content_length)
        {
            // TODO: write to file
            cout << data << endl;
            data = string(data, content_length, buf_pos - content_length);
            return;
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
            string version;

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
}

void Client::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(function.c_str());
        exit(1);
    }
}
