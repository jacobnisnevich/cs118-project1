#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "http-response.h"

const time_t TIMEOUT_SEC = 5;

class Server
{
public:
    Server(const char* host, const char* port);
    void accept_connections();

private:
	static void process_request(int socket_fd);
	static void send_200_resp(int fd, bool keep_alive, struct stat buf, std::string version);
	static void send_404_resp(int fd, std::string version);
	static void send_405_resp(int fd, std::string version);
    static void process_error(int status, std::string function);
    int m_sockfd;
};

#endif //SERVER_H
