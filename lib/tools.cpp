#include "tools.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

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

std::string readFile(const string& file)
{
    try {
        if (!filesystem::exists(file))
            throw runtime_error("File does not exists");
        ifstream ifs(file, ios::in);
        ifs.exceptions(ifs.exceptions() | ios::failbit | ios::badbit);
        stringstream ss;
        ss << ifs.rdbuf();
        return ss.str();
    } catch (const exception& e) {
        throw runtime_error((stringstream() << "Cannot read file: \"" << file << "\". Error: " << e.what()).str());
    }
}

void writeFile(const std::string& file, const std::string& content)
{
    try {
        ofstream ofs(file, ios::out);
        ofs.exceptions(ofs.exceptions() | ios::failbit | ios::badbit);
        ofs << content;
    } catch (const exception& e) {
        throw runtime_error((stringstream() << "Cannot write file: \"" << file << "\". Error: " << e.what()).str());
    }
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
