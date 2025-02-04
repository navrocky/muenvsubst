#include "tools.h"

#include <iostream>
#include <iterator>

using namespace std;

KeyValue parseKeyValue(const std::string& s)
{
    auto i = s.find("=");
    if (i == std::string::npos)
        return { .key = s };
    return {
        .key = s.substr(0, i),
        .value = s.substr(i + 1),
    };
}

std::string readAllInput()
{
    cin >> noskipws;
    istream_iterator<char> it(cin);
    istream_iterator<char> end;
    return string(it, end);
}

vector<string> stringSplit(const string& s, const string& delimiter)
{
    vector<string> res;
    if (delimiter.empty()) {
        res.push_back(s);
        return res;
    }
    string::size_type prevPos = 0, pos = 0;
    while ((pos = s.find(delimiter, pos)) != string::npos) {
        res.push_back(s.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }
    res.push_back(s.substr(prevPos, pos - prevPos));
    return res;
}
