#include "client.h"
#include <string>
#include <errno.h>
#include <iostream>

Client::Client(const char *host, const char *port)
{
    socklen_t m_addr_size;
    struct sockaddr_storage m_accepted_addr;
    struct addrinfo hints;
    struct addrinfo *res;
    int m_sockfd;
    int status;

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
