#include "inja_renderer.h"

#include <inja/inja.hpp>
#include <sstream>
#include <stdexcept>

#include "tools.h"

using namespace std;
using namespace std::placeholders;

inline void toLower(string& res)
{
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return std::tolower(c); });
}

inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

inja::json split(const inja::Arguments& args)
{
    if (args.size() != 2)
        throw runtime_error("Expected 2 parameters: split(s: string, delimiter: string): string[]");
    auto s = args[0]->get<string>();
    auto delimiter = args[1]->get<string>();
    return stringSplit(s, delimiter);
}

inja::json varToBool(const inja::Arguments& args, inja::json envs)
{
    if (args.size() != 1)
        throw runtime_error("Expected 1 parameters: toBool(varName: string): bool");
    auto name = args[0]->get<string>();
    auto val = envs[name];
    if (val.is_null())
        return false;
    if (val.is_boolean())
        return val.get<bool>();
    if (val.is_string()) {
        auto s = val.get<string>();
        toLower(s);
        ltrim(s);
        rtrim(s);
        return s == "true" || s == "yes" || s == "on" || s == "1";
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

string renderWithInja(const string& tmpl, char** envp)
{
    auto envs = getAllEnvs<inja::json>(envp);
    inja::Environment env;
    env.add_callback("split", bind(split, _1));
    env.add_callback("error", bind(error, _1));
    env.add_callback("varToBool", [&](const inja::Arguments& args) { return varToBool(args, envs); });
    return env.render(tmpl, envs);
}
