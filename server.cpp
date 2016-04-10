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

    // set up socket calls
    status = getaddrinfo(host, port, &hints, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    // find socket to bind to
    auto i = res;
    for (; i != NULL; i = i->ai_next)
    {
        status = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) 
        if (status == -1) continue;

        sockfd = socket(res->ai_family, res->ai_socktype, 0);
        if (sockfd == -1) continue;

        status = bind(sockfd, res->ai_addr, res->ai_addrlen);
        if (status == -1) continue;

        break;
    }

    freeaddrinfo(res);

    // check if reached end of linked list
    if (i == NULL)
    {
        cout << "could not bind to any sockets"
        exit(1);
    }

    // start listening
    status = listen(sockfd, 10);
    process_error(status, "listen");
}

bool Server::accept_connections()
{
    while (1)
    {
        struct sockaddr_storage accepted_addr;
        socklen_t addr_size = sizeof(accepted_addr);
        int new_fd = accept(sockfd, (struct sockaddr *) &accepted_addr, &addr_size);
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