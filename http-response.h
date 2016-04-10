#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <vector>
#include <stdint.h>

class HttpResponse
{
    void get_status_code(std::string status_code);
    void get_status_message(std::string status_message);
    void set_connection(std::string connection);

    std::string get_status_code()
    std::string get_status_message()
    std::string get_connection()

    std::string create_response_string();

    // Encode HttpResponse into string of bytes
    std::string encode();

    // Decode string of bytes to HttpResponse
    void consume(std::string response);
private:
    std::string m_status_code;
    std::string m_status_message;
    std::string m_connection;
};

#endif
