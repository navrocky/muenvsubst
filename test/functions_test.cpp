// clazy:excludeall=non-pod-global-static
#include <catch2/catch_all.hpp>

#include <lib/inja_renderer.h>

using namespace Catch::Matchers;

static char* emptyArgs[] = { nullptr };

TEST_CASE("varToBool", "[functions]")
{
    SECTION("Set true variable and check")
    {
        auto res = renderWithInja(R"({% set MY_VAR="yes" %}{{ default(MY_VAR, null) | toBool }})");
        CHECK(res == "true");
    }
    SECTION("Check for undefind variable")
    {
        auto res = renderWithInja(R"({{ default(MY_VAR, null) | toBool }})");
        CHECK(res == "false");
    }
    auto tmpl = R"({{ varToBool("MY_VAR") }})";
    SECTION("Non existing variable")
    {
        auto res = renderWithInja(tmpl);
        CHECK(res == "false");
    }
    SECTION("Existing true variable")
    {
        auto res = renderWithInja(tmpl, { .envs = { { "MY_VAR", "yes" } } });
        CHECK(res == "true");
    }
    SECTION("Existing false variable")
    {
        auto res = renderWithInja(tmpl, { .envs = { { "MY_VAR", "no" } } });
        CHECK(res == "false");
    }
}

TEST_CASE("fromJson", "[functions]")
{
    SECTION("Valid JSON")
    {
        auto res = renderWithInja(R"({{ fromJson(" { \"key\" : \"value\" } ") }})");
        CHECK(res == R"({"key":"value"})");
    }
    SECTION("Invalid JSON")
    {
        CHECK_THROWS_WITH(renderWithInja(R"({{ fromJson(" { \"key\" } ") }})"), ContainsSubstring("Cannot parse JSON"));
    }
    SECTION("Get value from JSON")
    {
        auto res = renderWithInja(R"(## set json = fromJson(" { \"key\" : { \"innerKey\": \"innerValue\" } } ")
{{ json.key.innerKey }})");
        CHECK(res == "innerValue");
    }
}

TEST_CASE("toJson", "[functions]")
{
    SECTION("Single line")
    {
        auto json = renderWithInja(R"({{ " { \"key\" : \"value\" } " | fromJson | toJson }})");
        CHECK(json == R"({"key":"value"})");
    }
    SECTION("Indented")
    {
        auto json = renderWithInja(R"({{ " { \"key\" : \"value\" } " | fromJson | toJson(2) }})");
        CHECK(json == R"({
  "key": "value"
})");
    }
}

TEST_CASE("base64", "[functions]")
{
    SECTION("To base64")
    {
        auto res = renderWithInja(R"({{ toBase64("hello") }})");
        CHECK(res == "aGVsbG8=");
    }
    SECTION("From base64")
    {
        auto res = renderWithInja(R"({{ fromBase64("aGVsbG8=") }})");
        CHECK(res == "hello");
    }
    SECTION("Encode error")
    {
        CHECK_THROWS_WITH(renderWithInja(R"({{ fromBase64("+u====") }})"), ContainsSubstring("Cannot decode base64"));
    }
}
