#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server
{
public:
    Server(char* host, char* port, char* dir);
    bool accept_connections();
    void process_error(int status, std::string function);

private:
    static void process_request(int fd);
    char* m_dir;
    int m_sockfd;
};

#endif //SERVER_H