// prompt.h - Custom shell prompt with Git branch

#ifndef PROMPT_H
#define PROMPT_H

#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <cstdio>

std::string getGitBranch() {
    FILE* pipe = popen("git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (!pipe) return "";
    
    char buffer[128];
    std::string branch;
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        branch = buffer;
        branch.erase(branch.find_last_not_of(" \n\r\t")+1);  // Trim newline
    }
    pclose(pipe);
    return branch;
}

void displayPrompt() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    const char* username = getpwuid(getuid())->pw_name;
    std::string branch = getGitBranch();

    std::cout << username << "@mysh:" << cwd;
    if (!branch.empty()) {
        std::cout << " (git:" << branch << ")";
    }
    std::cout << "$ ";
}

#endif

