#include "http-request.h"
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <netdb.h>

using namespace std;

int main(int argc, char* argv[])
{
	// HttpRequest req;
	// req.setMethod("GET");
	// req.setUrl("/");
	// req.setHost("www.google.com");
	// req.setUserAgent("Mozilla/5.0");
	// req.setConnection("close");

	// string requestString = req.createRequestString();

	// cout << requestString << endl;

	const string DEFIP = "localhost";
	const string DEFPORT = "4000";
	const string DEFDIR = ".";

	vector<char> ip;
	vector<char> port;
	vector<char> dir;
	string tmp;

	argv[1] ? tmp = string(argv[1]) : tmp = DEFIP;
	copy(tmp.begin(), tmp.end(), ip.begin());

	argv[2] ? tmp = string(argv[2]) : tmp = DEFPORT;
	copy(tmp.begin(), tmp.end(), port.begin());

	argv[3] ? tmp = string(argv[3]) : tmp = DEFDIR;
	copy(tmp.begin(), tmp.end(), dir.begin());

	//basic socket creation
	struct sockaddr_in addr;
	struct sockaddr_in *res;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.ai_socktype = SOCK_STREAM;
	addr.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, "4000", &addr, &res);
	int sockfd = socket(res->ai_family, res->ai_socktype, 0);
	bind(sockfd, res->ai_addr, res->ai_addrlen);
	connect(sockfd, res->ai_addr, res->ai_addrlen);
}
