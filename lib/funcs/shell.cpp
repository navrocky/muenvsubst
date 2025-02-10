#include "shell.h"

#include <stdexcept>

#include "../process_executor.h"

using namespace std;

namespace InjaFunctions {

inja::json shell(const inja::Arguments& args)
{
    if (args.size() == 1) {
        auto command = args[0]->get<string>();
        auto res = ProcessExecutor::executeAndCheckResult(command, "");
        return res.stdOut;
    } else if (args.size() == 2) {
        auto stdin = args[0]->get<string>();
        auto command = args[1]->get<string>();
        auto res = ProcessExecutor::executeAndCheckResult(command, stdin);
        return res.stdOut;
    } else {
        throw runtime_error("Expected 1 or 2 parameters: sh(stdin: string?, command: string): string");
    }
}

}
