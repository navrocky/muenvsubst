#pragma once

#include <inja/inja.hpp>

namespace InjaFunctions {

inja::json fromBase64(const inja::Arguments& args);
inja::json toBase64(const inja::Arguments& args);

}
