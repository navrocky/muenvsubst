#include "inja_renderer.h"

#include <inja/inja.hpp>
#include <stdexcept>

#include "tools.h"

using namespace std;
using namespace std::placeholders;

inja::json split(const inja::Arguments& args)
{
    if (args.size() != 2)
        throw runtime_error("Expected 2 parameters: split(s: string, delimiter: string)");
    auto s = args[0]->get<string>();
    auto delimiter = args[1]->get<string>();
    return stringSplit(s, delimiter);
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
    return env.render(tmpl, envs);
}
