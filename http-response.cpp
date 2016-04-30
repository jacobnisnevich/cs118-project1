#include "http-response.h"
#include <string>
#include <vector>
#include <cctype>
#include <regex>
#include <iostream>
#include <algorithm>

using namespace std;

HttpResponse::HttpResponse() 
{
    m_content_length = "";
}

void HttpResponse::set_version(string version) 
{
    m_version = version;
}

void HttpResponse::set_status_code(string status_code) 
{
    m_status_code = status_code;
}

void HttpResponse::set_status_message(string status_message) 
{
    m_status_message = status_message;
}

void HttpResponse::set_connection(string connection) 
{
    m_connection = connection;
}

void HttpResponse::set_content_length(string content_length)
{
    m_content_length = content_length;
}

string HttpResponse::get_version() 
{
    return m_version;
}

string HttpResponse::get_status_code() 
{
    return m_status_code;
}

string HttpResponse::get_status_message()
{
    return m_status_message;
}

string HttpResponse::get_connection()
{
    return m_connection;
}

string HttpResponse::get_content_length()
{
    return m_content_length;
}

string HttpResponse::encode()
{
    string response_string = "HTTP/" + m_version + " " + m_status_code + 
        " " + m_status_message + "\r\n" +
        "Connection: " + m_connection + "\r\n";

    if (m_content_length != "")
    {
        response_string += "Content-Length: " + m_content_length + "\r\n";
    }

    response_string += "\r\n";
    
    return response_string;
}

bool HttpResponse::consume(string response)
{
    char* response_string = strdup(response.c_str());
    int line_count = 0;

    regex httpRegex("HTTP\\/(.*?) (.*?) (.*)");
    regex headerRegex("(.*?): (.*)");
    smatch match;

    char* line = strtok(response_string, "\r\n");
    while (line != 0)
    {
        string temp = string(line);
        if (line_count == 0)
        {
            if (regex_search(temp, match, httpRegex))
            {
                m_version = match[1];
                m_status_code = match[2];
                m_status_message = match[3];
            }
            else 
            {
                cerr << "Invalid Response." << endl;
                return false;
            }
        }
        else 
        {
            if (regex_search(temp, match, headerRegex))
            {
                string header_string = match[1];
                transform(header_string.begin(), header_string.end(),
                    header_string.begin(), ::tolower);

                if (header_string == "content-length")
                {
                    m_content_length = match[2];
                }
            }
            else 
            {
                cerr << "Invalid header." << endl;
                return false;
            }
        }

        line = strtok(NULL, "\r\n");
        line_count++;
    }

    free(response_string);

    return true;
}
