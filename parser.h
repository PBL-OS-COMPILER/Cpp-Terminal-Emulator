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

#include <iostream>

using namespace std;

vector<vector<string>> parsePipelinedCommand(const string &input, bool &background) {
    vector<vector<string>> pipedCommands;
    background = false;
    string trimmedInput = input;
    if (!trimmedInput.empty() && trimmedInput.back() == '&') {
        background = true;
        trimmedInput.pop_back();
    }

    istringstream pipeStream(trimmedInput);
    string segment;
    while (getline(pipeStream, segment, '|')) {
        istringstream iss(segment);
        vector<string> args;
        string token;
        while (iss >> token) {
            args.push_back(token);
        }
        if (!args.empty()) pipedCommands.push_back(args);
    }
    return pipedCommands;
}

#endif

