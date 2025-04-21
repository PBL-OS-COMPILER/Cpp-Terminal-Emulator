// executor.h - Command Execution and Redirection (Member 3)

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void executeCommand(const vector<string> &args, bool background, const string &inputFile, const string &outputFile, bool append) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    }

    if (pid == 0) {
        if (!inputFile.empty()) {
            int fd = open(inputFile.c_str(), O_RDONLY);
            if (fd < 0) { perror("input redirection error"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (!outputFile.empty()) {
            int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
            int fd = open(outputFile.c_str(), flags, 0644);
            if (fd < 0) { perror("output redirection error"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        vector<char*> execArgs;
        for (const string &arg : args) execArgs.push_back(const_cast<char*>(arg.c_str()));
        execArgs.push_back(nullptr);

        execvp(execArgs[0], execArgs.data());
        perror("exec error");
        exit(1);
    } else {
        if (!background) waitpid(pid, nullptr, 0);
        else cout << "Started in background with PID: " << pid << endl;
    }
}

#endif

