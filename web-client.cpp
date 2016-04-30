#include "client.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <iostream>

using namespace std;

url_t parse_url(string url);
void print_urls_map(map<pair<string, string>, vector<string> > urls);

int main(int argc, char* argv[])
{
    map<pair<string, string>, vector<string> > urls;

    for (int i = 1; i < argc; i++)
    {
        url_t temp = parse_url(argv[i]);

        urls[make_pair(temp.host, temp.port)].push_back(temp.file_path);
    }

    // print_urls_map(urls);

    Client client(urls, argc - 1);
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

    string host = url_parsed.host;
    if (regex_search(host, match, host_regex))
    {
        url_parsed.host = match[1];
        url_parsed.port = match[2];
    }

    return url_parsed;
}

void print_urls_map(map<pair<string, string>, vector<string> > urls)
{
    for (auto i : urls)
    {
        cout << "Host: " << i.first.first + ":" + i.first.second << endl;

        for (size_t j = 0; j < i.second.size(); j++)
        {
            cout << j << endl;
            cout << "\t File Path: " << i.second[j] << endl;
        }
    }
}
