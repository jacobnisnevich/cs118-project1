#include "http-request.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

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

	regex httpRegex("(.*?)\\s(.*?) HTTP\\/1\\.0\\r\\nHost: (.*?)\\r\\nUser-agent: (.*?)\\r\\nConnection: (.*?)\\r\\n\\r\\n");
	smatch match;

	if (regex_search(request, match, httpRegex))
	{
		m_method = match[1];
		m_url = match[2];
		m_host = match[3];
		m_userAgent = match[4];
		m_connection = match[5];
	}
	else 
	{
		cerr << "Invalid request." << endl;
	}
}

string HttpRequest::createRequestString() {
	return m_method + " " + m_url + " HTTP/1.0\r\n" +
		"Host: " + m_host + "\r\n" +
		"User-agent: " + m_userAgent + "\r\n" +
		"Connection: " + m_connection + "\r\n" +
		"\r\n";
}
