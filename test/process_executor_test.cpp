// clazy:excludeall=non-pod-global-static
#include <catch2/catch_all.hpp>

#include <lib/process_executor.h>

TEST_CASE("Process executor")
{
    SECTION("Success templating with sed")
    {
        auto res = ProcessExecutor::execute("sed 's/John/Kate/'", "Hello John!");
        CHECK(res.stdOut == "Hello Kate!");
    }
    SECTION("Exit with status")
    {
        auto res = ProcessExecutor::execute("exit 10", "");
        CHECK(res.stdOut == "");
        CHECK(res.stdErr == "");
        CHECK(res.result == 10);
    }
    SECTION("Get output")
    {
        auto res = ProcessExecutor::execute("echo -n 'abc'", "");
        CHECK(res.stdOut == "abc");
        CHECK(res.stdErr == "");
        CHECK(res.result == 0);
    }
    SECTION("Get error output")
    {
        auto res = ProcessExecutor::execute("echo -n 'abc' 1>&2", "");
        CHECK(res.stdOut == "");
        CHECK(res.stdErr == "abc");
        CHECK(res.result == 0);
    }
}
