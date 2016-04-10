#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server
{
public:
    Server(char* host, char* port, char* dir);
    bool accept_connections();
    bool process_request();
    void process_error(int status, std::string function);

private:
    char* m_dir;
    int m_sockfd;
};

#endif