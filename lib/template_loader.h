#pragma once

#include <string>
#include <vector>

class TemplateLoader {
public:
    ~TemplateLoader() { }
    virtual std::string load(const std::string& path) = 0;
};

class FileTemplateLoader : public TemplateLoader {
public:
    struct Opts {
        std::vector<std::string> includeDirs;
    };
    FileTemplateLoader(Opts&& opts);
    std::string load(const std::string& path) override;

private:
    std::string readRelativeFile(const std::string& path);
    Opts opts;
};
