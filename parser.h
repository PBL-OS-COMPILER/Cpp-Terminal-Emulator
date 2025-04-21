#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string> parseCommand(const string &input, bool &background, string &inputFile, string &outputFile, bool &append) {
    vector<string> args;
    istringstream iss(input);
    string token;
    background = false; append = false;
    while (iss >> token) {
        if (token == "&") background = true;
        else if (token == "<") iss >> inputFile;
        else if (token == ">") iss >> outputFile;
        else if (token == ">>") { append = true; iss >> outputFile; }
        else args.push_back(token);
    }
    return args;
}

#endif
