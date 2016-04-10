#include "http-request.h"
#include "server.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    const string DEFIP = "localhost";
    const string DEFPORT = "4000";
    const string DEFDIR = ".";
    string tmp;

    if (argc > 4)
    {
        cout << "Usage: " << argv[0] << " hostname port file-dir" << endl;
        exit(1);
    }
    
    string hostname = argc >= 2 ? argv[1] : DEFIP;
    string port = argc >= 3 ? argv[2] : DEFPORT;
    string dir = argc == 4 ? argv[3] : DEFDIR;

    Server server(&hostname[0], &port[0], &dir[0]);

    server.accept_connections();
}
