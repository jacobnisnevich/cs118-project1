#include "http-request.h"
#include <string>
#include <vector>

using namespace std;

void HttpRequest::setMethod(string method) {
	m_method = method;
}

void HttpRequest::setUrl(string url) {
	m_url = url;
}

void HttpRequest::setHost(string host) {
	m_host = host;
}

void HttpRequest::setUserAgent(string userAgent) {
	m_userAgent = userAgent;
}

void HttpRequest::setConnection(string connection) {
	m_connection = connection;
}

vector<uint8_t> HttpRequest::encode()
{
	string request = createRequestString();

	vector<uint8_t> wire(request.begin(), request.end());

	return wire;
}

void HttpRequest::consume(vector<uint8_t> wire)
{
	string request(wire.begin(), wire.end());

	// Parse request string to method, url, host, connection, and userAgent
}

string HttpRequest::createRequestString() {
	return m_method + " " + m_url + "HTTP/1.1\r\n" +
		"Host: " + m_host + "\r\n" +
		"User-agent: " + m_userAgent + "\r\n" +
		"Connection: " + m_connection + "\r\n" +
		"\r\n";
}
