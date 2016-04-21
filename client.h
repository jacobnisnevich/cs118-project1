#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <sys/types.h>

const time_t TIMEOUT_SEC = 5;

class Client
{
public:
    Client(std::string host, std::string port, std::string file_path);
    void process_response();

private:
	static void process_error(int status, std::string function);
	int m_sockfd;
};

#endif
