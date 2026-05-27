#pragma once 

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




class Shell {

        public : 

        Shell() ;
        void run() ;

        
        private :

            std::string PS1 = "$ " ;
            std::string PATH = getPath() ; 
            inline static std::unordered_set<std::string> builtins = {
                "echo",
                "exit",
                "type",  
                "pwd" , 
                "cd"
            };

             struct ParsedCommand {
                std::vector<std::string> args ; 
                std::string redirectFile ; 
                bool redirectStdout = false ;
            };

            static std::string getPath();
            void printLine(std::vector<std::string> &args);
            ParsedCommand parseLine(const std::string &input);
            std::string findExecutableInPath(const std::string &path , const std::string &arg);
            void handleTypeCommand(std::vector<std::string>&args);
            void executeProgram(const std::string &fullPath , std::vector<std::string> &args);
            void printCurrentDir() ;
            void changeDirectory(std::vector<std::string> &args);   
            int applyRedirect(const ParsedCommand &cmd);
            void restoreRedirect(int savedStdoutFd) ;
};

