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

class Shell {

        public:

        Shell();
        void run();

        private:

            
            static std::string getPath();
            inline static std::string PATH = getPath();
            
            inline static std::unordered_set<std::string> builtins = {
                "echo",
                "exit",
                "type",  
                "pwd" , 
                "cd" , 
                "complete" ,
            };

             struct ParsedCommand {
                std::vector<std::string> args ; 
                std::string redirectStdoutFile ; 
                std::string redirectStderrFile ;
                bool redirectStdout = false ;
                bool redirectStderr = false ;
                bool appendStdout = false;
                bool appendStderr = false;
            };

            struct SavedFds {
                int savedStdout = -1 ; 
                int savedStderr = -1 ;
            };

            inline static std::map<std::string, std::string> registeredCompletions ;

            void printLine(std::vector<std::string> &args);
            
            ParsedCommand parseLine(const std::string &input);
            std::vector<std::string> tokenize(const std::string &input);
            ParsedCommand parseRedirects(const std::vector<std::string> &words);

            
            std::string findExecutableInPath(const std::string &path, const std::string &arg);
            
            void handleTypeCommand(std::vector<std::string> &args);
            
            void executeProgram(const std::string &fullPath, std::vector<std::string> &args);
            
            void printCurrentDir();
            
            void changeDirectory(std::vector<std::string> &args);
            
            int redirectFile(const std::string &file, int target, bool append);
            SavedFds applyRedirect(const ParsedCommand &cmd);
            void restoreRedirect(SavedFds fds);
            
            static char* Completer(const char* text , int state) ;
            
            void handleCompleteCommand(std::vector<std::string> &args);
};

