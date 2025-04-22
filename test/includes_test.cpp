// clazy:excludeall=non-pod-global-static
#include <catch2/catch_all.hpp>

#include <lib/inja_renderer.h>

using namespace std;
using namespace Catch::Matchers;

static char* emptyArgs[] = { nullptr };

class MockedTemplateLoader : public TemplateLoader {
public:
    MockedTemplateLoader(const map<string, string>& files)
        : files(files)
    {
    }

    string load(const std::string& path) override
    {
        auto it = files.find(path);
        if (it == files.end())
            throw runtime_error("File not found: " + path);
        return it->second;
    }

private:
    map<string, string> files;
};

TEST_CASE("Include file", "[includes]")
{
    SECTION("Simple file")
    {
        MockedTemplateLoader loader = map<string, string> { { "path/simple_file", "simple file content" } };
        auto res = renderWithInja(R"({% include "path/simple_file" %})", { .templateLoader = &loader });
        CHECK(res == R"(simple file content)");
    }
    SECTION("File with vars")
    {
        MockedTemplateLoader loader = map<string, string> { { "file", "Hello {{ user }}!" } };
        auto res = renderWithInja(R"({% include "file" %})",
            {
                .envs = { { "user", "John" } },
                .templateLoader = &loader,
            });
        CHECK(res == R"(Hello John!)");
    }
}
