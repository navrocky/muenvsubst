#pragma once

#include <nlohmann/json.hpp>
#include <string>

#include "template_loader.h"

struct RenderOpts {
    nlohmann::json envs;
    TemplateLoader* templateLoader = nullptr;
};

std::string renderWithInja(const std::string& tmpl, const RenderOpts& opts = RenderOpts());
