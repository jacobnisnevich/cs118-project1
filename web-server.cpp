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

    if (argc > 4)
    {
        cout << "Usage: " << argv[0] << " hostname port file-dir" << endl;
        exit(1);
    }
    
    string hostname = argc >= 2 ? argv[1] : DEFIP;
    string port = argc >= 3 ? argv[2] : DEFPORT;
    string dir = argc == 4 ? argv[3] : DEFDIR;
    chdir(dir.c_str());

    Server server(hostname.c_str(), port.c_str());
    server.accept_connections();
}
