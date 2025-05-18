// main.cpp - Entry point for the Terminal Emulator

#include "parser.h"
#include "history.h"
#include "builtins.h"
#include "executor.h"

int main() {
    HistoryManager history;
    string input, inputFile, outputFile;
    bool background, append;

    enableRawMode();

    while (true) {
        cout << "$ ";
        input = readInputWithHistory(history);
        if (input.empty()) continue;

        history.add(input);

        inputFile = outputFile = "";
        vector<string> args = parseCommand(input, background, inputFile, outputFile, append);

        if (!handleBuiltInCommands(args)) {
            executeCommand(args, background, inputFile, outputFile, append);

#include "parser.h"
#include "pipeline_executor.h"
#include "history.h"
#include "prompt.h"
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    loadHistory();

    while (true) {
        displayPrompt();

        string input;
        char* line = readline("mysh> ");
        if (!line) break;
        input = string(line);
        if (!input.empty()) {
          add_history(line);     // For in-session history
          addToHistory(input);   // For persistent history (your code)
        }
       free(line);

        if (input.empty()) continue;

        addToHistory(input);
        bool background = false;
        auto commands = parsePipelinedCommand(input, background);

        if (commands.size() == 1) {
            // check for built-ins
            if (commands[0][0] == "cd") {
                if (commands[0].size() > 1) chdir(commands[0][1].c_str());
            } else if (commands[0][0] == "exit") {
                saveHistory();
                break;
            } else if (commands[0][0] == "clear") {
                system("clear");
            } else if (commands[0][0] == "history") {
                showHistory();
            } else {
                pid_t pid = fork();
                if (pid == 0) {
                    vector<char*> argv;
                    for (auto &arg : commands[0])
                        argv.push_back(const_cast<char*>(arg.c_str()));
                    argv.push_back(nullptr);
                    execvp(argv[0], argv.data());
                    perror("execvp");
                    exit(1);
                } else if (!background) {
                    waitpid(pid, nullptr, 0);
                }
            }
        } else {
            executePipelinedCommands(commands, background);
        }
    }

    return 0;
}

