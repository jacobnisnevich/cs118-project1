#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sys/types.h>

struct url_t {
    std::string host;
    std::string port;
    std::string file_path;
};

const time_t TIMEOUT_SEC = 5;

class Client
{
public:
    Client(std::unordered_map<std::string, std::vector<url_t> > urls, int n_urls);
    void process_response();

private:
	static void process_error(int status, std::string function);
	int m_sockfd;
};

#endif
