#include "shell.h"
#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>

namespace fs = std::filesystem;

using namespace std;


string PATH = getPath();

unordered_set<string> builtins = {
    "echo",
    "exit",
    "type"
};


string getPath() {
    char* p = getenv("PATH");
    return p ? string(p) : "";
}



void printLine(stringstream &params) {
    string rest ;
    getline(params , rest);

    if(!rest.empty() && rest[0] == ' ') {
        rest.erase(0,1);
    }

    cout << rest << endl ;
}


string findExecutableInPath(const string &path , const string &arg) {

    string dir ; 

    stringstream dirs(path);
    
    while(getline(dirs, dir, ':')) {

        // build fullPath 
    
        string fullPath = dir + "/" + arg ; 

        // if path exists and is executable
        
        if(fs::exists(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
            return fullPath ;
        }
        
    }

    return "" ; 
}



void handleTypeCommand(stringstream &params){

        string arg ;


    
    while(params >> arg) {
        if(builtins.count(arg)) {
            cout << arg << " is a shell builtin" << endl ;
        }
        else {


            string fullPath = findExecutableInPath(PATH , arg);
            
            if(!fullPath.empty()) {
                cout << arg << " is " << fullPath << endl ; 
         
            }
            else {
               cout << arg << ": not found" << endl ; 
            }
            
        }
    }
}