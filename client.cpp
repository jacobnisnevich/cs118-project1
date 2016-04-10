#include "client.h"
#include <errno.h>
#include <thread>
#include <iostream>

Client::Client()
{
    socklen_t m_addr_size;
    struct sockaddr_storage m_accepted_addr;
    struct addrinfo hints;
    int m_sockfd;
    struct addrinfo *res;
    int status;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "4000", &hints, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
    process_error(m_sockfd, "socket");

    status = connect(m_sockfd, res->ai_addr, res->ai_addrlen);
    process_error(status, "connect");
}

void Client::process_error(int status, string function)
{
    if (status == -1)
    {
        perror(function);
        exit(1);
    }
}
