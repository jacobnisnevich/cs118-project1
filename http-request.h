#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>

using namespace std;

class HttpRequest
{
public:
	void setMethod(string method);
	void setUrl(string url);
	void setHost(string host);
	void setUserAgent(string userAgent);
	void setConnection(string connection);

	string createRequestString();

	// Encode HttpRequest into string of bytes
	vector<uint8_t> encode();

	// Decode string of bytes to HttpRequest
	void consume(vector<uint8_t> wire);
private:
	string m_method;
	string m_url;
	string m_host;
	string m_userAgent;
	string m_connection;
};

#endif
