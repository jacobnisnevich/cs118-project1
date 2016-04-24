#include "client.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

vector<string> parse_url(string url);

int main(int argc, char* argv[])
{
	// TODO: allow for multiple requests

//	for (int i = 1; i < argc; i++)
//	{
//
//	}

    vector<string> params = parse_url(argv[1]);

    Client client(params[0], params[1], params[2]);
    client.process_response();
}

vector<string> parse_url(string url)
{
	regex url_regex("http://(.*?)(/.*)");
	smatch match;

	string host = "";
	string port = "";
	string file_path = "";

	if (regex_search(url, match, url_regex))
	{
		host = match[1];
		file_path = match[2];
	}
	else 
	{
		cerr << "Invalid url" << endl;
		exit(1);
	}

	regex host_regex("(.*?):(.*)");

	if (regex_search(host, match, host_regex))
	{
		host = match[1];
		port = match[2];
	}

	vector<string> params_vector;
	params_vector = {host, port, file_path};

	return params_vector;
}
