#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stdint.h>

class HttpRequest
{
public:
    void set_method(std::string method);
    void set_url(std::string url);
    void set_version(std::string version);

    std::string get_method();
    std::string get_url();
    std::string get_version();

    // Encode HttpRequest into string of bytes
    std::string encode();

    // Decode string of bytes to HttpRequest
    void consume(std::string wire);
private:
    // Variables for first line of request
    std::string m_method;
    std::string m_url;
    std::string m_version;

    // Hash of headers
    std::unordered_map<std::string, std::string> m_headers;
};

#endif
