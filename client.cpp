#include "client.h"
#include "http-request.h"
#include <string>
#include <cstring>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>

using namespace std;

Client::Client(string host, string port, string file_path)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    // TODO: loop through res linked list
    int sock_fd = socket(res->ai_family, res->ai_socktype, 0);
    process_error(sock_fd, "socket");

    status = connect(sock_fd, res->ai_addr, res->ai_addrlen);
    process_error(status, "connect");

    HttpRequest req;
    req.set_method("GET");
    req.set_url(file_path);
    req.set_version("1.0");

    string request = req.encode();
    send(sock_fd, request.c_str(), request.size(), 0);

    size_t buf_pos = 0;
    string data;
    data.resize(512);

    while (1)
    {
        // TODO: client should receive persistent responses
        int n_bytes = recv(sock_fd, &data[buf_pos], data.size() - buf_pos, 0);
        if (n_bytes == 0)
        {
            // Received EOF
            close(sock_fd);
            cout << data << endl;
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
            string wire(data, 0, req_end_pos + 4);
            data = string(data, req_end_pos + 4, buf_pos - (req_end_pos + 4));
            buf_pos = data.length();
            data.resize(512);

            cout << wire << endl;
        }
    }

    close(sock_fd);
}

void Client::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(function.c_str());
        exit(1);
    }
}
