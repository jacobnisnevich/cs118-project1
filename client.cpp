#include "client.h"
#include "http-request.h"
#include "http-response.h"
#include <string>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <iostream>

Client::Client(std::string host, std::string port, std::string file_path)
{
    struct addrinfo hints;
    struct addrinfo *res;
    int m_sockfd;
    int status;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (status != 0)
    {
        std::cout << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        exit(1);
    }

    m_sockfd = socket(res->ai_family, res->ai_socktype, 0);
    process_error(m_sockfd, "socket");

    status = connect(m_sockfd, res->ai_addr, res->ai_addrlen);
    process_error(status, "connect");

    HttpRequest req;
    req.set_method("GET");
    req.set_url(file_path);
    req.set_version("1.0");

    std::string request = req.encode();
    send(m_sockfd, request.c_str(), request.size(), 0);
}

void Client::process_error(int status, std::string function)
{
    if (status == -1)
    {
        perror(function.c_str());
        exit(1);
    }
}
