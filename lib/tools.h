#pragma once

#include <string>
#include <vector>

std::string readAllInput();

struct KeyValue {
    std::string key;
    std::string value;
};

KeyValue parseKeyValue(const std::string& s);

template <typename Map>
Map getAllEnvs(char** envp)
{
    Map m;
    for (char** env = envp; *env != 0; env++) {
        auto kv = parseKeyValue(*env);
        m[kv.key] = kv.value;
    }
    return m;
}

std::vector<std::string> stringSplit(const std::string& s, const std::string& delimiter);

void stringToLower(std::string& res);

void stringLTrim(std::string& s);

void stringRTrim(std::string& s);

bool stringToBool(std::string s);
