#include "inja_renderer.h"

#include <inja/inja.hpp>

#include "funcs/base64.h"
#include "funcs/json.h"
#include "funcs/other.h"
#include "funcs/shell.h"
#include "funcs/string.h"

using namespace std;
using namespace std::placeholders;

string renderWithInja(const string& tmpl, const RenderOpts& opts)
{
    inja::Environment env;

    // configure functions
    env.add_callback("split", bind(InjaFunctions::split, _1));
    env.add_callback("error", bind(InjaFunctions::error, _1));
    env.add_callback("varToBool", bind(InjaFunctions::varToBool, _1, opts.envs));
    env.add_callback("toBool", bind(InjaFunctions::toBool, _1));
    env.add_callback("sh", bind(InjaFunctions::shell, _1));
    env.add_callback("trim", bind(InjaFunctions::trim, _1));
    env.add_callback("fromJson", bind(InjaFunctions::fromJson, _1));
    env.add_callback("toJson", bind(InjaFunctions::toJson, _1));
    env.add_callback("toBase64", bind(InjaFunctions::toBase64, _1));
    env.add_callback("fromBase64", bind(InjaFunctions::fromBase64, _1));

    // configure file loader
    env.set_include_callback([&env, &opts](const std::string& path, const std::string& templateName) {
        if (!opts.templateLoader)
            throw runtime_error("Template loader not configured");
        auto templateContent = opts.templateLoader->load(templateName);
        return env.parse(templateContent);
    });

    return env.render(tmpl, opts.envs);
}
