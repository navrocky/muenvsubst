#include "process_executor.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

#include "finalize.h"

using namespace std;

namespace ProcessExecutor {

namespace {

void closeFd(int& fd)
{
    if (fd >= 0)
        close(fd);
    fd = -1;
}

string readAll(int fd)
{
    const int bufSize = 100;
    char buf[bufSize];
    stringstream ss;
    for (;;) {
        int rd = read(fd, buf, bufSize);
        ss.write(buf, rd);
        if (rd < bufSize)
            break;
    }
    return ss.str();
};

}

ExecutionResult execute(const std::string& cmd, const std::string& cmdInput)
{
    const char* shell = getenv("SHELL");

    int in_fd[2];
    int out_fd[2];
    int err_fd[2];

    pipe(in_fd); // For child's stdin
    pipe(out_fd); // For child's stdout
    pipe(err_fd); // For child's stderr

    finalize
    {
        closeFd(in_fd[0]);
        closeFd(in_fd[1]);
        closeFd(out_fd[0]);
        closeFd(out_fd[1]);
        closeFd(err_fd[0]);
        closeFd(err_fd[1]);
    };

    // Fork
    pid_t pid = fork();
    if (pid == -1) {
        throw runtime_error("Cannot fork");
    }

    if (pid == 0) {
        // We're in the child
        closeFd(out_fd[0]);
        dup2(out_fd[1], STDOUT_FILENO);
        closeFd(out_fd[1]);

        closeFd(err_fd[0]);
        dup2(err_fd[1], STDERR_FILENO);
        closeFd(err_fd[1]);

        closeFd(in_fd[1]);
        dup2(in_fd[0], STDIN_FILENO);
        closeFd(in_fd[0]);

        if (execl(shell, shell, "-c", cmd.c_str(), nullptr) == -1) {
            // error of execv
            _exit(1);
        }
        throw runtime_error("This is not where we're supposed to be");
    } else {
        // You're in the parent
        closeFd(out_fd[1]);
        closeFd(err_fd[1]);
        closeFd(in_fd[0]);

        if (!cmdInput.empty()) {
            write(in_fd[1], cmdInput.data(), cmdInput.size());
            closeFd(in_fd[1]);
        }

        // Wait for the child to terminate (or it becomes a zombie)
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            throw runtime_error("waitpid error");
        }

        int es = 0;
        if (WIFEXITED(status)) {
            es = WEXITSTATUS(status);
        } else {
            throw runtime_error("Process not exited");
        }

        ExecutionResult res;
        res.result = es;
        res.stdOut = readAll(out_fd[0]);
        res.stdErr = readAll(err_fd[0]);
        return res;
    }
}

ExecutionResult executeAndCheckResult(const std::string& cmd, const string& input)
{
    auto result = execute(cmd, input);
    checkExecutionResult(result, cmd);
    return result;
}

void checkExecutionResult(const ExecutionResult& result, const std::string& cmd)
{
    if (result.result != 0)
        throw runtime_error((stringstream()
            << "<7cc999ce> Execution process failed. command=\"" << cmd << "\", result=" << result.result
            << ", stdOut=\"" << result.stdOut << "\", stdErr=\"" << result.stdErr << "\"")
                .str());
}
}
