#include "http-request.h"
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <unordered_map>
#include <iostream>

using namespace std;

void HttpRequest::set_method(string method)
{
	m_method = method;
}

void HttpRequest::set_url(string url)
{
	m_url = url;
}

void HttpRequest::set_version(string version)
{
	m_version = version;
}

void HttpRequest::set_header(string header, string value)
{
	m_headers[header] = value;
}

string HttpRequest::get_method()
{
	return m_method;
}

string HttpRequest::get_url()
{
	return m_url;
}

string HttpRequest::get_version()
{
	return m_version;
}

std::string HttpRequest::get_header(std::string header)
{
	return m_headers[header];
}

string HttpRequest::encode()
{
	string request_string = m_method + " " + m_url + " HTTP/" + m_version + "\r\n";

	for (auto i : m_headers)
	{
		request_string += i.first + ": " + i.second + "\r\n";
	}

	request_string += "\r\n";

	return request_string;
}

void HttpRequest::consume(string request)
{
	char* request_cstr = new char[request.length() + 1];
	strcpy(request_cstr, request.c_str());

	int line_count = 0;

	regex httpRegex("(.*?) (.*?) HTTP\\/(.*)");
	regex headerRegex("(.*?): (.*)");
	smatch match;

	char* line = strtok(request_cstr, "\r\n");
	while (line != 0)
	{
		if (line_count == 0)
		{
			if (regex_search(string(line), match, httpRegex))
			{
				m_method = match[1];
				m_url = match[2];
				m_version = match[3];
			}
			else 
			{
				cerr << "Invalid request." << endl;
			}
		}
		else 
		{
			if (regex_search(string(line), match, headerRegex))
			{
				m_headers[match[1]] = match[2];
			}
			else 
			{
				cerr << "Invalid header." << endl;
			}
		}

		line = strtok(NULL, "\r\n");
		line_count++;
	}

	delete[] request_cstr;
	delete[] line;
}
