#include "http-response.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

void HttpResponse::set_status_code(string status_code) {
	m_status_code = status_code;
}

void HttpResponse::set_status_message(string status_message) {
	m_status_message = status_message;
}

void HttpResponse::set_connection(string connection) {
	m_connection = connection;
}

void HttpResponse::set_status_code(string status_code) {
	m_status_code = status_code;
}

void HttpResponse::set_status_message(string status_message) {
	m_status_message = status_message;
}

void HttpResponse::set_connection(string connection) {
	m_connection = connection;
}

string HttpResponse::create_response_string() {
	return m_method + " " + m_url + " HTTP/1.0\r\n" +
		"Host: " + m_host + "\r\n" +
		"User-agent: " + m_user_agent + "\r\n" +
		"Connection: " + m_connection + "\r\n" +
		"\r\n";
}

vector<uint8_t> HttpResponse::encode()
{
	string request = create_request_string();

	vector<uint8_t> wire(request.begin(), request.end());

	return create_request_string();
}

void HttpResponse::consume(vector<uint8_t> wire)
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
