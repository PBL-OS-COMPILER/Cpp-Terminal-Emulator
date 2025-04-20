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
        }
    }

    return 0;
}

