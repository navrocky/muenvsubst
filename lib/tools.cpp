#include "tools.h"

#include <algorithm>
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

void stringToLower(std::string& res)
{
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return tolower(c); });
}

void stringLTrim(std::string& s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
}

void stringRTrim(std::string& s)
{
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
}

bool stringToBool(std::string s)
{
    stringToLower(s);
    stringLTrim(s);
    stringRTrim(s);
    return s == "true" || s == "yes" || s == "on" || s == "1";
}
