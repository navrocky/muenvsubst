// clazy:excludeall=non-pod-global-static
#include <catch2/catch_all.hpp>

#include <lib/inja_renderer.h>

using namespace Catch::Matchers;

static char* emptyArgs[] = { nullptr };

TEST_CASE("fromJson", "[functions]")
{
    SECTION("Valid JSON")
    {
        auto res = renderWithInja(R"({{ fromJson(" { \"key\" : \"value\" } ") }})", emptyArgs);
        CHECK(res == R"({"key":"value"})");
    }
    SECTION("Invalid JSON")
    {
        CHECK_THROWS_WITH(
            renderWithInja(R"({{ fromJson(" { \"key\" } ") }})", emptyArgs), ContainsSubstring("Cannot parse JSON"));
    }
    SECTION("Get value from JSON")
    {
        auto res = renderWithInja(R"(## set json = fromJson(" { \"key\" : { \"innerKey\": \"innerValue\" } } ")
{{ json.key.innerKey }})",
            emptyArgs);
        CHECK(res == "innerValue");
    }
}

TEST_CASE("toJson", "[functions]")
{
    SECTION("Single line")
    {
        auto json = renderWithInja(R"({{ " { \"key\" : \"value\" } " | fromJson | toJson }})", emptyArgs);
        CHECK(json == R"({"key":"value"})");
    }
    SECTION("Indented")
    {
        auto json = renderWithInja(R"({{ " { \"key\" : \"value\" } " | fromJson | toJson(2) }})", emptyArgs);
        CHECK(json == R"({
  "key": "value"
})");
    }
}

TEST_CASE("base64", "[functions]")
{
    SECTION("To base64")
    {
        auto res = renderWithInja(R"({{ toBase64("hello") }})", emptyArgs);
        CHECK(res == "aGVsbG8=");
    }
    SECTION("From base64")
    {
        auto res = renderWithInja(R"({{ fromBase64("aGVsbG8=") }})", emptyArgs);
        CHECK(res == "hello");
    }
    SECTION("Encode error")
    {
        CHECK_THROWS_WITH(
            renderWithInja(R"({{ fromBase64("+u====") }})", emptyArgs), ContainsSubstring("Cannot decode base64"));
    }
}
