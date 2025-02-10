#pragma once

#include <string>

#include <ext/stdio_filebuf.h> // NB: Specific to libstdc++
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <exception>


namespace ProcessExecutor {

struct ExecutionResult {
    int result;
    std::string stdOut;
    std::string stdErr;
};

ExecutionResult execute(const std::string& cmd, const std::string& cmdInput = std::string());
void checkExecutionResult(const ExecutionResult& result, const std::string& cmd);
ExecutionResult executeAndCheckResult(const std::string& cmd, const std::string& input = std::string());

}

// Wrapping pipe in a class makes sure they are closed when we leave scope
class cpipe {
private:
    int fd[2];
public:
    const inline int read_fd() const { return fd[0]; }
    const inline int write_fd() const { return fd[1]; }
    cpipe() { if (pipe(fd)) throw std::runtime_error("Failed to create pipe"); }
    void close() { ::close(fd[0]); ::close(fd[1]); }
    ~cpipe() { close(); }
};

class spawn {
private:
    cpipe write_pipe;
    cpipe read_pipe;
public:
    int child_pid = -1;
    std::unique_ptr<__gnu_cxx::stdio_filebuf<char> > write_buf = NULL;
    std::unique_ptr<__gnu_cxx::stdio_filebuf<char> > read_buf = NULL;
    std::ostream stdin;
    std::istream stdout;

    spawn(const char* const argv[], bool with_path = false, const char* const envp[] = 0): stdin(NULL), stdout(NULL) {
        child_pid = fork();
        if (child_pid == -1) throw std::runtime_error("Failed to start child process");
        if (child_pid == 0) {   // In child process
            dup2(write_pipe.read_fd(), STDIN_FILENO);
            dup2(read_pipe.write_fd(), STDOUT_FILENO);
            write_pipe.close(); read_pipe.close();
            int result;
            if (with_path) {
                if (envp != 0) result = execvpe(argv[0], const_cast<char* const*>(argv), const_cast<char* const*>(envp));
                else result = execvp(argv[0], const_cast<char* const*>(argv));
            }
            else {
                if (envp != 0) result = execve(argv[0], const_cast<char* const*>(argv), const_cast<char* const*>(envp));
                else result = execv(argv[0], const_cast<char* const*>(argv));
            }
            if (result == -1) {
                // Note: no point writing to stdout here, it has been redirected
                std::cerr << "Error: Failed to launch program" << std::endl;
                exit(1);
            }
        }
        else {
            close(write_pipe.read_fd());
            close(read_pipe.write_fd());
            write_buf = std::unique_ptr<__gnu_cxx::stdio_filebuf<char> >(new __gnu_cxx::stdio_filebuf<char>(write_pipe.write_fd(), std::ios::out));
            read_buf = std::unique_ptr<__gnu_cxx::stdio_filebuf<char> >(new __gnu_cxx::stdio_filebuf<char>(read_pipe.read_fd(), std::ios::in));
            stdin.rdbuf(write_buf.get());
            stdout.rdbuf(read_buf.get());
        }
    }

    void send_eof() { write_buf->close(); }

    int wait() {
        int status;
        waitpid(child_pid, &status, 0);
        return status;
    }
};
