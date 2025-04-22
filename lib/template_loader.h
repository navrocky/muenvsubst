#pragma once

#include <string>

class TemplateLoader {
public:
    ~TemplateLoader() { }
    virtual std::string load(const std::string& path) = 0;
};

class FileTemplateLoader : public TemplateLoader {
public:
    std::string load(const std::string& path) override;
};
