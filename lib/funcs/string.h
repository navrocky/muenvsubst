#pragma once

#include <inja/inja.hpp>

namespace InjaFunctions {

inja::json split(const inja::Arguments& args);
inja::json trim(const inja::Arguments& args);
inja::json toBool(const inja::Arguments& args);
inja::json parseJson(const inja::Arguments& args);

}
