#include "inja_renderer.h"

#include <inja/inja.hpp>

#include "funcs/other.h"
#include "funcs/shell.h"
#include "funcs/string.h"
#include "tools.h"

using namespace std;
using namespace std::placeholders;

string renderWithInja(const string& tmpl, char** envp)
{
    auto envs = getAllEnvs<inja::json>(envp);
    inja::Environment env;
    env.add_callback("split", bind(InjaFunctions::split, _1));
    env.add_callback("error", bind(InjaFunctions::error, _1));
    env.add_callback("varToBool", [&](const inja::Arguments& args) { return InjaFunctions::varToBool(args, envs); });
    env.add_callback("toBool", [&](const inja::Arguments& args) { return InjaFunctions::toBool(args); });
    env.add_callback("sh", [&](const inja::Arguments& args) { return InjaFunctions::shell(args); });
    env.add_callback("trim", [&](const inja::Arguments& args) { return InjaFunctions::trim(args); });
    return env.render(tmpl, envs);
}
