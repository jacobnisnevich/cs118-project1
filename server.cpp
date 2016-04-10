#include "server.h"
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <thread>
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>

using namespace std;

Server::Server(char* host, char* port, char* dir)
{
    m_dir = dir;

    struct addrinfo hints;
    struct addrinfo *res;
    int status;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(host, port, &hints, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
    process_error(m_sockfd, "socket");

    status = bind(m_sockfd, res->ai_addr, res->ai_addrlen);
    process_error(status, "bind");

    status = listen(m_sockfd, 10);
    process_error(status, "listen");

    freeaddrinfo(res);
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
            cout << "accepted" << endl;
            close(new_fd);
        }
    }
    return true;
}

bool Server::process_request()
{
    return true;
}

void Server::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(&function[0]);
        exit(1);
    }
}