#include "http-request.h"
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <unordered_map>
#include <iostream>
#include <sstream>

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

string HttpRequest::get_header(string header)
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

bool HttpRequest::consume(string request)
{
    regex httpRegex("(.*?) (.*?) HTTP\\/(.*)");
    regex headerRegex("(.*?): (.*)");
    smatch match;

    istringstream iss(move(request));
    string line;

    // regex first line
    getline(iss, line);
    line.pop_back();
    if (regex_search(line, match, httpRegex))
    {
        m_method = match[1];
        m_url = match[2];
        m_version = match[3];
    }
    else
    {
        cerr << "Invalid request." << endl;
        return false;
    }

    // regex headers
    while (getline(iss, line))
    {
        if (line.size() == 0)
        {
            continue;
        }

        line.pop_back();
        if (regex_search(line, match, headerRegex))
        {
            string header_key = match[1];
            transform(header_key.begin(), header_key.end(),
                header_key.begin(), ::tolower);

            string header_val = match[2];
            transform(header_val.begin(), header_val.end(),
                header_val.begin(), ::tolower);

            m_headers[header_key] = header_val;
        }
        else 
        {
            cerr << "Invalid header." << endl;
            cerr << line << endl;
            return false;
        }
    }

    return true;
}
