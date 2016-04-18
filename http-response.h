#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <vector>
#include <stdint.h>

class HttpResponse
{
public:
    HttpResponse();
    
	void set_version(std::string version);
    void set_status_code(std::string status_code);
    void set_status_message(std::string status_message);
    void set_connection(std::string connection);
    void set_content_length(std::string content_length);

    std::string get_version();
    std::string get_status_code();
    std::string get_status_message();
    std::string get_connection();
    std::string get_content_length();

    // Encode HttpResponse into string of bytes
    std::string encode();
private:
	std::string m_version;
    std::string m_status_code;
    std::string m_status_message;
    std::string m_connection;
    std::string m_content_length;
};

#endif
