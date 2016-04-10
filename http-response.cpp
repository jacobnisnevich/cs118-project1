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

string HttpResponse::get_status_code() {
    return m_status_code;
}

string HttpResponse::get_status_message() {
    return m_status_message;
}

string HttpResponse::set_connection() {
    return m_connection;
}

string HttpResponse::create_response_string() {
    return "HTTP/1.0 " + m_status_code + " " + m_status_message + "\r\n" +
        "Connection: " + m_connection + "\r\n" +
        "\r\n";
}

string HttpResponse::encode()
{
    return create_response_string();
}

void HttpResponse::consume(string response)
{
    regex httpRegex("HTTP\\/1\\.0 (.*?) (.*?)\\r\\nConnection: (.*?)\\r\\n\\r\\n");
    smatch match;

    if (regex_search(response, match, httpRegex))
    {
        m_status_code = match[1];
        m_status_message = match[2];
        m_connection = match[5];
    }
    else 
    {
        cerr << "Invalid response." << endl;
    }
}
