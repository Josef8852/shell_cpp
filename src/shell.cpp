#include "shell.h"

using namespace std;

namespace fs = filesystem;


Shell::Shell() {}


void Shell::run() {

    while(true) {

        cout << PS1;

        string line;

        getline(cin, line);

        ParsedCommand cmd = parseLine(line);

        if(cmd.args.empty()) continue;

        string command = cmd.args[0];

        if(command == "exit") break;

        SavedFds fds = applyRedirect(cmd);

        if(command == "echo") printLine(cmd.args);

        else if(command == "pwd") printCurrentDir();

        else if(command == "type") handleTypeCommand(cmd.args);

        else if(command == "cd") changeDirectory(cmd.args);

        else {

            string fullPath = findExecutableInPath(PATH, command);

            if(!fullPath.empty()) {
                executeProgram(fullPath, cmd.args);
            }
            else {
                cout << command << ": command not found" << endl;
            }
        }

        restoreRedirect(fds);
    }
}


string Shell::getPath() {
    char* p = getenv("PATH");
    return p ? string(p) : "";
}


Shell::ParsedCommand Shell::parseLine(const string &input) {
    vector<string> words;
    string word;
    bool isInsideSingleQuote = false, isInsideDoubleQuotes = false, isEscaping = false;

    for(char c : input) {

        if(isEscaping) {
            word += c;
            isEscaping = false;
            continue;
        }

        if(c == '\\' && !isInsideSingleQuote) {
            isEscaping = true;
            continue;
        }

        if(c == '\'' && !isInsideDoubleQuotes) {
            isInsideSingleQuote = !isInsideSingleQuote;
            continue;
        }

        if(c == '\"' && !isInsideSingleQuote) {
            isInsideDoubleQuotes = !isInsideDoubleQuotes;
            continue;
        }

        if(c == ' ' && !isInsideSingleQuote && !isInsideDoubleQuotes) {
            if(!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }

    if(!word.empty()) {
        words.push_back(word);
    }

    ParsedCommand result;

    for(size_t i = 0; i < words.size(); i++) {
        if(words[i] == ">" || words[i] == "1>" || words[i] == ">>" || words[i] == "1>>") {

            result.redirectStdout = true;
            result.appendStdout = (words[i] == ">>" || words[i] == "1>>");
            if(i+1 < words.size()) {
                result.redirectStdoutFile = words[i+1];
                i++;
            }

            continue;
        }

        if(words[i] == "2>" || words[i] == "2>>") {
            result.redirectStderr = true;
            result.appendStderr = words[i] == "2>>";
            if(i+1 < words.size()) {
                result.redirectStderrFile = words[i+1];
                i++;
            }
            continue;
        }

        result.args.push_back(words[i]);
    }

    return result;
}


void Shell::printLine(vector<string> &args) {

    for(size_t i = 1; i < args.size(); i++) {
        if(i > 1) cout << " ";
        cout << args[i];
    }

    cout << endl;
}


string Shell::findExecutableInPath(const string &path, const string &arg) {

    string dir;
    stringstream dirs(path);

    while(getline(dirs, dir, ':')) {

        string fullPath = dir + "/" + arg;

        if(fs::exists(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
            return fullPath;
        }
    }

    return "";
}


void Shell::handleTypeCommand(vector<string> &args) {

    for(size_t i = 1; i < args.size(); i++) {

        string arg = args[i];

        if(builtins.count(arg)) {
            cout << arg << " is a shell builtin" << endl;
        }
        else {
            string fullPath = findExecutableInPath(PATH, arg);

            if(!fullPath.empty()) {
                cout << arg << " is " << fullPath << endl;
            }
            else {
                cout << arg << ": not found" << endl;
            }
        }
    }
}


void Shell::executeProgram(const string &fullPath, vector<string> &args) {

    vector<char*> argv;

    for(auto &str : args) {
        argv.push_back(str.data());
    }

    argv.push_back(nullptr);

    pid_t pid = fork();

    if(pid == 0) {
        execv(fullPath.c_str(), argv.data());
        perror("execv failed");
        exit(1);
    }
    else if(pid > 0) {
        waitpid(pid, nullptr, 0);
    }
    else {
        perror("fork failed");
    }
}


void Shell::printCurrentDir() {
    cout << fs::current_path().string() << endl;
}

void Shell::changeDirectory(vector<string> &args) {

    char* home = getenv("HOME");

    if(!home) {
        cout << "cd: Home not set" << endl;
        return;
    }

    if(args.size() == 1 || args[1] == "~") {
        fs::current_path(string(home));
        return;
    }

    string dir = args[1];

    if(fs::exists(dir) && fs::is_directory(dir)) {
        fs::current_path(dir);
    }
    else {
        cout << "cd: " << dir << ": No such file or directory" << endl;
    }
}


int Shell::redirectFile(const string &file, int target, bool append) {
    int fd = open(file.c_str(), O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);

    if(fd < 0) {
        perror("Failed to open the file");
        exit(1);
    }

    int savedFd = dup(target);
    dup2(fd, target);
    close(fd);
    return savedFd;
}

Shell::SavedFds Shell::applyRedirect(const ParsedCommand &cmd) {

    SavedFds fds;

    if(cmd.redirectStdout && !cmd.redirectStdoutFile.empty()) {
        fds.savedStdout = redirectFile(cmd.redirectStdoutFile, STDOUT_FILENO, cmd.appendStdout);
    }

    if(cmd.redirectStderr && !cmd.redirectStderrFile.empty()) {
        fds.savedStderr = redirectFile(cmd.redirectStderrFile, STDERR_FILENO, cmd.appendStderr);
    }

    return fds;
}


void Shell::restoreRedirect(SavedFds fds) {

    if(fds.savedStdout >= 0) {
        dup2(fds.savedStdout, STDOUT_FILENO);
        close(fds.savedStdout);
    }

    if(fds.savedStderr >= 0) {
        dup2(fds.savedStderr, STDERR_FILENO);
        close(fds.savedStderr);
    }
}
