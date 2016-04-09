#include "server.h"
#include <errno.h>
#include <thread>
#include <iostream>

Server::Server()
{
    socklen_t m_addr_size;
    struct sockaddr_storage m_accepted_addr;
    struct addrinfo m_addr;
    int m_sockfd;
    struct addrinfo *res;
    int status;

    m_addr.ai_family = AF_INET;
    m_addr.ai_socktype = SOCK_STREAM;
    m_addr.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, "4000", &m_addr, &res);
    if (status != 0)
    {
        cout << "getaddrinfo error: " << gai_strerror(status) << endl;
        exit(1);
    }

    for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
    // convert address to IPv4 address
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

    // convert the IP to a string and print it:
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
    std::cout << "  " << ipstr << std::endl;
    // std::cout << "  " << ipstr << ":" << ntohs(ipv4->sin_port) << std::endl;
  }


    m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
    process_error(m_sockfd, "socket");

    status = bind(m_sockfd, res->ai_addr, res->ai_addrlen);
    process_error(status, "bind");

    status = connect(m_sockfd, res->ai_addr, res->ai_addrlen);
    process_error(status, "connect");

    status = listen(m_sockfd, 10);
    process_error(status, "listen");

    //remember to free res
}

bool Server::accept_connections()
{
    socklen_t addr_size = sizeof(m_accepted_addr);
    int new_fd = accept(m_sockfd, (struct sockaddr *) &m_accepted_addr, &addr_size);
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
        perror(function);
        exit(1);
    }
}