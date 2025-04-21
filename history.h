#ifndef HISTORY_H
#define HISTORY_H

#include <deque>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#define MAX_HISTORY 50
using namespace std;

class HistoryManager {
    deque<string> history;
    int historyIndex = -1;
public:
    void add(const string &cmd) {
        if (history.size() == MAX_HISTORY) history.pop_front();
        history.push_back(cmd);
        historyIndex = history.size();
    }

    string navigateUp() {
        if (historyIndex > 0) return history[--historyIndex];
        return "";
    }

    string navigateDown() {
        if (historyIndex < (int)history.size() - 1) return history[++historyIndex];
        historyIndex = history.size();
        return "";
    }
};

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

string readInputWithHistory(HistoryManager &history) {
    string input;
    char c;
    string current = "";

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == '\n') {
            cout << endl;
            break;
        } else if (c == 27) { // escape sequence
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[') {
                if (seq[1] == 'A') { // up
                    cout << "\33[2K\r$ ";
                    current = history.navigateUp();
                    cout << current;
                    input = current;
                    cout.flush();
                } else if (seq[1] == 'B') { // down
                    cout << "\33[2K\r$ ";
                    current = history.navigateDown();
                    cout << current;
                    input = current;
                    cout.flush();
                }
            }
        } else if (c == 127) { // backspace
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        } else {
            input += c;
            cout << c;
            cout.flush();
        }
    }
    return input;
}

#endif

