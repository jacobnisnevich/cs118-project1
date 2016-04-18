#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
    Client(std::string host, std::string port, std::string file_path);

private:
	static void process_error(int status, std::string function);
};

#endif
