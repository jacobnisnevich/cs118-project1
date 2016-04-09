#include "http-request.h"
#include <string>
#include <thread>
#include <iostream>

using namespace std;

int main()
{
    HttpRequest req;
    req.set_method("GET");
    req.set_url("/");
    req.set_host("www.google.com");
    req.set_user_agent("Mozilla/5.0");
    req.set_connection("close");

    string requestString = req.create_request_string();
    cout << requestString << endl;

    req.consume(req.encode());

    requestString = req.create_request_string();
    cout << requestString << endl;
}
