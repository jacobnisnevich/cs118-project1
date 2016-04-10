#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <vector>
#include <stdint.h>

class HttpResponse
{
public:
    void set_method(std::string method);
    void set_url(std::string url);
    void set_host(std::string host);
    void set_user_agent(std::string user_agent);
    void set_connection(std::string connection);

    std::string get_method();
    std::string get_url();
    std::string get_host();
    std::string get_user_agent();
    std::string get_connection();

    std::string create_response_string();

    // Encode HttpResponse into string of bytes
    std::vector<uint8_t> encode();

    // Decode string of bytes to HttpResponse
    void consume(std::vector<uint8_t> wire);
private:
    std::string m_method;
    std::string m_url;
    std::string m_host;
    std::string m_user_agent;
    std::string m_connection;
};

#endif
