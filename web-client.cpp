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

    // TODO: CHANGE SO THAT IT ACCEPTS HOST AND PORT
    Client client(argv[0]);
}