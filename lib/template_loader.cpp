#include "template_loader.h"

#include "tools.h"

std::string FileTemplateLoader::load(const std::string& path) { return readFile(path); }
