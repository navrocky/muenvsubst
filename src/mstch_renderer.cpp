#include "mstch_renderer.h"

#include <mstch/mstch.hpp>

#include "tools.h"

std::string renderWithMstch(const std::string& tmpl, char** envp)
{
    auto envs = getAllEnvs<mstch::map>(envp);
    return mstch::render(tmpl, envs);
}
