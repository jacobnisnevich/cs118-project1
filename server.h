#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server
{
public:
    Server(const char* host, const char* port);
    bool accept_connections();
    void process_error(int status, std::string function);

private:
    static void process_request(int fd);
    int m_sockfd;
};

#endif //SERVER_H