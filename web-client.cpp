#include "client.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " url" << endl;
        exit(1);
    }

    Client client(argv[0]);
}