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

    argv[1] ? tmp = string(argv[1]) : tmp = DEFIP;
    const vector<char> ip(tmp.begin(), tmp.end());

    argv[2] ? tmp = string(argv[2]) : tmp = DEFPORT;
    const vector<char> port(tmp.begin(), tmp.end());

    argv[3] ? tmp = string(argv[3]) : tmp = DEFDIR;
    const vector<char> dir(tmp.begin(), tmp.end());
}
