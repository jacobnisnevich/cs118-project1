#include "http-request.h"
#include <string>
#include <thread>
#include <iostream>

using namespace std;

int main()
{
	HttpRequest req;
	req.setMethod("GET");
	req.setUrl("/");
	req.setHost("www.google.com");
	req.setUserAgent("Mozilla/5.0");
	req.setConnection("close");

	string requestString = req.createRequestString;

	cout << requestString << endl;
}
