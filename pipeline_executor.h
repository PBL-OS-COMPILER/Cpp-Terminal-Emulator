#ifndef PIPELINE_EXECUTOR_H
#define PIPELINE_EXECUTOR_H

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

void executePipelinedCommands(const vector<vector<string>> &commands, bool background) {
    int numCommands = commands.size();
    int pipefd[2], in_fd = 0;

    for (int i = 0; i < numCommands; ++i) {
        if (i < numCommands - 1) pipe(pipefd);

        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(in_fd, 0);
                close(in_fd);
            }
            if (i < numCommands - 1) {
                dup2(pipefd[1], 1);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            vector<char *> argv;
            for (const string &arg : commands[i])
                argv.push_back(const_cast<char *>(arg.c_str()));
            argv.push_back(nullptr);

            execvp(argv[0], argv.data());
            perror("execvp failed");
            exit(1);
        } else {
            if (i > 0) close(in_fd);
            if (i < numCommands - 1) {
                close(pipefd[1]);
                in_fd = pipefd[0];
            }
            if (!background) waitpid(pid, nullptr, 0);
        }
    }
}

#endif

