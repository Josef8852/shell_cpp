#pragma once 

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <fcntl.h>
#include <readline/readline.h>
#include <functional>
#include <unordered_map>

class Shell {
public:
    Shell();
    void run();

private:
    //  Types 
    struct ParsedCommand {
        std::vector<std::string> args;
        std::string redirectStdoutFile;
        std::string redirectStderrFile;
        bool redirectStdout = false;
        bool redirectStderr = false;
        bool appendStdout   = false;
        bool appendStderr   = false;
    };

    struct SavedFds {
        int savedStdout = -1;
        int savedStderr = -1;
    };

    using Handler = std::function<void(ParsedCommand&)>;

    // ── Static data ──────────────────────────────────────────
    static std::string getPath();
    inline static std::string PATH = getPath();

    inline static std::unordered_set<std::string> builtins_ = {
        "echo", "exit", "type", "pwd", "cd", "complete", "jobs",
    };

    inline static std::map<std::string, std::string> registeredCompletions_;

    //  Instance data
    std::unordered_map<std::string, Handler> handlers_;

    //  Parsing 
    ParsedCommand parseLine(const std::string &input);
    std::vector<std::string> tokenize(const std::string &input);
    ParsedCommand parseRedirects(const std::vector<std::string> &words);

    //  Builtins 
    void printLine(std::vector<std::string> &args);
    void printCurrentDir();
    void changeDirectory(std::vector<std::string> &args);
    void handleTypeCommand(std::vector<std::string> &args);
    void handleCompleteCommand(std::vector<std::string> &args);
    void handleJobs();

    //  Execution
    std::string findExecutableInPath(const std::string &path, const std::string &arg);
    void executeProgram(const std::string &fullPath, std::vector<std::string> &args);

    //  Redirection 
    int redirectFile(const std::string &file, int target, bool append);
    SavedFds applyRedirect(const ParsedCommand &cmd);
    void restoreRedirect(SavedFds fds);

    //  Completion 
    static char* Completer(const char* text, int state);
};