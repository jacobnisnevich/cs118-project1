#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <sys/types.h>

struct url_t
{
    std::string host;
    std::string port;
    std::string file_path;
};

const time_t TIMEOUT_SEC = 5;

class Client
{
public:
    Client(std::map<std::pair<std::string, std::string>, std::vector<std::string> > urls, int n_urls);
private:
	std::string get_file_name(std::string file_path);
	void connect_to_socket(const char* host, const char* port);
    std::string process_response(std::string file_name);
	static void process_error(int status, std::string function);
	int m_sockfd;
};

#endif
