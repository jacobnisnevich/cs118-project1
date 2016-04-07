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

	// TODO: DEBUG SEGMENTATION FAULT
	const char DEFPORT[5] = "4000";
	const char DEFDIR[2] = ".";

	char *node = nullptr;
	char *service;
	char *directory;

	// setting host name
	if (argv[1]) {
		size_t len = strlen(argv[1]);
		node = (char *)(malloc(len * sizeof(char) + 1));
		strncpy(node, argv[1], len);
	}

	// setting port number
	if (argv[2]) {
		size_t len = strlen(argv[2]);
		service = (char *)(malloc(len * sizeof(char) + 1));
		strncpy(service, argv[2], len);
	}
	else {
		size_t len = strlen(DEFPORT);
		service = (char *)(malloc(len * sizeof(char) + 1));
		strncpy(service, DEFPORT, len);
	}

	// set file-directory
	if (argv[2]) {
		size_t len = strlen(argv[3]);
		directory = (char *)(malloc(len * sizeof(char) + 1));
		strncpy(directory, argv[3], len);
	}
	else {
		size_t len = strlen(DEFDIR);
		directory = (char *)(malloc(len * sizeof(char) + 1));
		strncpy(directory, DEFDIR, len);
	}

	cout << node << endl;
	cout << service << endl;
	cout << directory << endl;

	// basic socket creation
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
