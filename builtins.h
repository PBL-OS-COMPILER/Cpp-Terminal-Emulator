// builtins.h - Built-in Command Handler (Member 2)

#ifndef BUILTINS_H
#define BUILTINS_H

#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

bool handleBuiltInCommands(const vector<string> &args) {
    if (args.empty()) return true;

    if (args[0] == "exit") {
        cout << "Exiting terminal...\n";
        exit(0);
    } else if (args[0] == "cd") {
        const char *path = args.size() > 1 ? args[1].c_str() : getenv("HOME");
        if (chdir(path) != 0) perror("cd error");
        return true;
    } else if (args[0] == "clear") {
        cout << "\033[H\033[J";
        return true;
    }
    return false;
}

#endif

