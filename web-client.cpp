#include "client.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

std::vector<std::string> parse_url(std::string url);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " url" << std::endl;
        exit(1);
    }

    std::vector<std::string> params = parse_url(argv[1]);

    Client client(params[0], params[1], params[2]);
    client.process_response();
}

std::vector<std::string> parse_url(std::string url)
{
	std::regex url_regex("http://(.*?)(/.*)");
	std::smatch match;

	std::string host = "";
	std::string port = "";
	std::string file_path = "";

	if (std::regex_search(url, match, url_regex))
	{
		host = match[1];
		file_path = match[2];
	}
	else 
	{
		std::cerr << "Invalid url." << std::endl;
		exit(1);
	}

	std::regex host_regex("(.*?):(.*)");

	if (std::regex_search(host, match, host_regex))
	{
		host = match[1];
		port = match[2];
	}

	std::vector<std::string> params_vector;
	params_vector = {host, port, file_path};

	return params_vector;
}
