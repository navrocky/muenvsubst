#include "template_loader.h"

#include <filesystem>

#include "tools.h"

using namespace std;

FileTemplateLoader::FileTemplateLoader(Opts&& opts)
    : opts(std::move(opts))
{
}

std::string FileTemplateLoader::load(const std::string& path)
{
    if (filesystem::path(path).is_relative())
        return readRelativeFile(path);
    else
        return readFile(path);
}

string FileTemplateLoader::readRelativeFile(const std::string& path)
{
    for (const auto& dir : opts.includeDirs) {
        auto p = filesystem::path(dir).append(path);
        if (filesystem::exists(p))
            return readFile(p);
    }
    throw runtime_error((stringstream() << "File not found: \"" << path << "\"").str());
}
