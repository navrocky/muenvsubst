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
    env.add_callback("varToBool", bind(InjaFunctions::varToBool, _1, envs));
    env.add_callback("toBool", bind(InjaFunctions::toBool, _1));
    env.add_callback("sh", bind(InjaFunctions::shell, _1));
    env.add_callback("trim", bind(InjaFunctions::trim, _1));
    env.add_callback("parseJson", bind(InjaFunctions::parseJson, _1));
    return env.render(tmpl, envs);
}
