#include "other.h"

#include <iostream>

#include "../tools.h"

using namespace std;

namespace InjaFunctions {

inja::json varToBool(const inja::Arguments& args, inja::json envs)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: toBool(varName: string): bool");
    auto name = args[0]->get<string>();
    cerr << "Warning: varToBool is deprecated. Use: 'default(" << name << ", null) | toBool' instead";
    auto val = envs[name];
    if (val.is_null())
        return false;
    if (val.is_boolean())
        return val.get<bool>();
    if (val.is_string()) {
        auto s = val.get<string>();
        return stringToBool(s);
    }
    throw runtime_error((stringstream() << "Unsupported env value type for name " << name).str());
}

inja::json error(const inja::Arguments& args)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: error(message: string)");
    auto message = args[0]->get<string>();
    throw runtime_error(message);
}

}
