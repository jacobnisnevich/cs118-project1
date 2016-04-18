#include "http-response.h"
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using namespace std;

HttpResponse::HttpResponse() {
    m_content_length = "";
}

void HttpResponse::set_version(string version) {
    m_version = version;
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

void HttpResponse::set_content_length(string content_length) {
    m_content_length = content_length;
}

string HttpResponse::get_version() {
    return m_version;
}

string HttpResponse::get_status_code() {
    return m_status_code;
}

string HttpResponse::get_status_message() {
    return m_status_message;
}

string HttpResponse::get_connection() {
    return m_connection;
}

string HttpResponse::get_content_length() {
    return m_content_length;
}

string HttpResponse::encode() {
    string response_string = "HTTP/" + m_version + " " + m_status_code + 
        " " + m_status_message + "\r\n" +
        "Connection: " + m_connection + "\r\n";

    if (m_content_length != "")
    {
        response_string += "Content-length: " + m_content_length + "\r\n";
    }

    response_string += "\r\n";
    
    return response_string;
}
