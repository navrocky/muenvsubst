#pragma once

#include <inja/inja.hpp>

namespace InjaFunctions {

inja::json fromJson(const inja::Arguments& args);
inja::json toJson(const inja::Arguments& args);

}
