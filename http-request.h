#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

class HttpRequest
{
public:
    void set_method(string method);
    void set_url(string url);
    void set_host(string host);
    void set_user_agent(string user_agent);
    void set_connection(string connection);

    string get_method();
    string get_url();
    string get_host();
    string get_user_agent();
    string get_connection();

    string create_request_string();

    // Encode HttpRequest into string of bytes
    vector<uint8_t> encode();

    // Decode string of bytes to HttpRequest
    void consume(vector<uint8_t> wire);
private:
    string m_method;
    string m_url;
    string m_host;
    string m_user_agent;
    string m_connection;
};

#endif
