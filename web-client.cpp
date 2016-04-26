#include "client.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <iostream>

using namespace std;

struct url_t {
    string host;
    string port;
    string file_path;
};

url_t parse_url(string url);
void print_urls_map(unordered_map<string, vector<url_t> > urls);

int main(int argc, char* argv[])
{
    unordered_map<string, vector<url_t> > urls;

    for (int i = 1; i < argc; i++)
    {
        url_t temp = parse_url(argv[i]);

        urls[temp.host].push_back(temp);
    }

    print_urls_map(urls);

    // vector<string> params = parse_url(argv[1]);

    // Client client(params[0], params[1], params[2]);
    // client.process_response();
}

url_t parse_url(string url)
{
    regex url_regex("http://(.*?)(/.*)");
    smatch match;

    url_t url_parsed;

    url_parsed.host = "";
    url_parsed.port = "80";
    url_parsed.file_path = "";

    if (regex_search(url, match, url_regex))
    {
        url_parsed.host = match[1];
        url_parsed.file_path = match[2];
    }
    else 
    {
        cerr << "Invalid url" << endl;
        exit(1);
    }

    regex host_regex("(.*?):(.*)");

    if (regex_search(url_parsed.host, match, host_regex))
    {
        url_parsed.host = match[1];
        url_parsed.port = match[2];
    }

    return url_parsed;
}

void print_urls_map(unordered_map<string, vector<url_t> > urls)
{
    for (auto i : urls)
    {
        cout << "Host: " << i.first << endl;

        for (size_t j = 0; j < i.second.size(); j++)
        {
            cout << j << endl;
            cout << "\t Port: " << i.second[j].port << endl;
            cout << "\t File Path: " << i.second[j].file_path << endl;
        }
    }
}
