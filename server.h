#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <netdb.h>

class Server
{
public:
    Server();
    bool accept_connections();
    bool process_request();
    void process_error(int status, string function);

private:
};

#endif