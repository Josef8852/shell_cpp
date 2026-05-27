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

            static std::string getPath();
            void printLine(std::vector<std::string> &args);
            std::vector<std::string> parseLine(const std::string &input);
            std::string findExecutableInPath(const std::string &path , const std::string &arg);
            void handleTypeCommand(std::vector<std::string>&args);
            void executeProgram(const std::string &fullPath , std::vector<std::string> &args);
            void printCurrentDir() ;
            void changeDirectory(std::vector<std::string> &args);   
};

