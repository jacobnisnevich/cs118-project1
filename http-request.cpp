#include "http-request.h"
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <unordered_map>
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

string HttpRequest::encode()
{
	return create_request_string();
}

void HttpRequest::consume(string request)
{
	char* request_cstr = new char[request.length() + 1];
	strcpy(request_cstr, request.c_str());

	unordered_map<string, string> headers;

	int line_count = 0;

	regex httpRegex("(.*?) (.*?) HTTP\\/1\\.0");
	regex headerRegex("(.*?): (.*?)");
	smatch match;

	char* line = strtok(request_cstr, "\r\n");
	while (line != 0)
	{
		if (line_count == 0)
		{
			if (regex_search(request, match, httpRegex))
			{
				m_method = match[1];
				m_url = match[2];
			}
			else 
			{
				cerr << "Invalid request." << endl;
			}
		}
		else 
		{
			if (regex_search(request, match, headerRegex))
			{
				headers[match[1]] = match[2];
			}
			else 
			{
				cerr << "Invalid header." << endl;
			}
		}

		line = strtok(NULL, "\r\n");
	}

	if (headers.count("Host") > 0)
	{
		m_host = headers["Host"];
	}
	if (headers.count("User-agent") > 0)
	{
		m_user_agent = headers["User-agent"];
	}
	if (headers.count("Connection") > 0)
	{
		m_connection = headers["Connection"];
	}

	delete[] request_cstr;
	delete[] line;
}
