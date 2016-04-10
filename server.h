#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "http-response.h"

class Server
{
public:
    Server(const char* host, const char* port);
    bool accept_connections();
    void process_error(int status, std::string function);

private:
	static void send_404_resp(HttpResponse resp, int fd);
    static void process_request(int socket_fd);
    int m_sockfd;
};

#endif //SERVER_H