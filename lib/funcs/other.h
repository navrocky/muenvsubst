#pragma once

#include <inja/inja.hpp>

namespace InjaFunctions {

inja::json varToBool(const inja::Arguments& args, inja::json envs);
inja::json error(const inja::Arguments& args);

}
