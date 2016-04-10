#include "http-request.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

void HttpRequest::set_method(string method) {
	m_method = method;
}

void HttpRequest::set_url(string url) {
	m_url = url;
}

void HttpRequest::set_host(string host) {
	m_host = host;
}

void HttpRequest::set_user_agent(string user_agent) {
	m_user_agent = user_agent;
}

void HttpRequest::set_connection(string connection) {
	m_connection = connection;
}

string HttpRequest::get_method() {
	return m_method;
}

string HttpRequest::get_url() {
	return m_url;
}

string HttpRequest::get_host() {
	return m_host;
}

string HttpRequest::get_user_agent() {
	return m_user_agent;
}

string HttpRequest::get_connection() {
	return m_connection;
}

string HttpRequest::create_request_string() {
	return m_method + " " + m_url + " HTTP/1.0\r\n" +
		"Host: " + m_host + "\r\n" +
		"User-agent: " + m_user_agent + "\r\n" +
		"Connection: " + m_connection + "\r\n" +
		"\r\n";
}

vector<uint8_t> HttpRequest::encode()
{
	string request = create_request_string();

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
		m_user_agent = match[4];
		m_connection = match[5];
	}
	else 
	{
		cerr << "Invalid request." << endl;
	}
}
