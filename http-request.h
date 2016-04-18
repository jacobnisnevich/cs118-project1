#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <stdint.h>

class HttpRequest
{
public:
    void set_method(std::string method);
    void set_url(std::string url);
    void set_version(std::string version);
    void set_host(std::string host);
    void set_user_agent(std::string user_agent);
    void set_connection(std::string connection);

    std::string get_method();
    std::string get_url();
    std::string get_version();
    std::string get_host();
    std::string get_user_agent();
    std::string get_connection();

    std::string create_request_string();

    // Encode HttpRequest into string of bytes
    std::string encode();

    // Decode string of bytes to HttpRequest
    void consume(std::string wire);
private:
    std::string m_method;
    std::string m_url;
    std::string m_version;
    std::string m_host;
    std::string m_user_agent;
    std::string m_connection;
};

#endif
