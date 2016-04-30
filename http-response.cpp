#include "http-response.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

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

void HttpResponse::consume(string response)
{
    char* response_cstr = new char[response.length() + 1];
    strcpy(response_cstr, response.c_str());

    int line_count = 0;

    regex httpRegex("HTTP\\/(.*?) (.*?) (.*)");
    regex headerRegex("(.*?): (.*)");
    smatch match;

    char* line = strtok(response_cstr, "\r\n");
    while (line != 0)
    {
        if (line_count == 0)
        {
            if (regex_search(string(line), match, httpRegex))
            {
                m_version = match[1];
                m_status_code = match[2];
                m_status_message = match[3];
            }
            else 
            {
                cerr << "Invalid Response." << endl;
            }
        }
        else 
        {
            if (regex_search(string(line), match, headerRegex))
            {
                if (match[1] == "Content-length")
                {
                    m_content_length = match[2];
                }
            }
            else 
            {
                cerr << "Invalid header." << endl;
            }
        }

        line = strtok(NULL, "\r\n");
        line_count++;
    }

    delete[] response_cstr;
    delete[] line;
}
