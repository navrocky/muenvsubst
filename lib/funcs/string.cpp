#include "string.h"

#include "../tools.h"
#include <nlohmann/json.hpp>

using namespace std;

namespace InjaFunctions {

inja::json split(const inja::Arguments& args)
{
    if (args.size() != 2)
        throw runtime_error("Expected 2 parameters: split(s: string, delimiter: string): string[]");
    auto s = args[0]->get<string>();
    auto delimiter = args[1]->get<string>();
    return stringSplit(s, delimiter);
}

inja::json trim(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: trim(text: string): string");
    auto text = args[0]->get<string>();
    stringLTrim(text);
    stringRTrim(text);
    return text;
}

inja::json toBool(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: toBool(value: any): bool");
    auto val = args[0];
    if (val->is_null())
        return false;
    if (val->is_boolean())
        return val->get<bool>();
    if (val->is_number())
        return val->get<int>() != 0;
    if (val->is_string()) {
        auto s = val->get<string>();
        return stringToBool(s);
    }
    throw runtime_error((stringstream() << "Unsupported value type").str());
}

inja::json parseJson(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: parseJson(json: string): json");
    auto jsonString = args[0]->get<string>();
    try {
        auto json = inja::json::parse(jsonString);
        return json;
    } catch (const exception& e) {
        throw runtime_error((stringstream() << "Cannot parse JSON: " << e.what()).str());
    }
}

}
