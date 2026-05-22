#include "shell.h"
#include <cstdlib>
#include <filesystem>
#include <sstream>



string PATH = getPath();

unordered_set<string> builtins = {
    "echo",
    "exit",
    "type",  
    "pwd" , 
    "cd"
};


string getPath() {
    char* p = getenv("PATH");
    return p ? string(p) : "";
}



vector<string> parseLine(const string &input) {
    vector<string> words;
    string word;
    bool isInsideSingleQuote = false;

    for (char c : input) {

        if (c == '\'') {
            isInsideSingleQuote = !isInsideSingleQuote;
            continue;
        }

        if (c == ' ' && !isInsideSingleQuote) {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } 
        else {
            word += c;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}


void printLine(vector<string> &args) {

    for(int i = 1 ; i < args.size() ; i++) {
        cout << args[i] << " ";
    }

    cout << endl ; 
    
}


string findExecutableInPath(const string &path , const string &arg) {

    string dir ; 

    stringstream dirs(path);
    
    while(getline(dirs, dir, ':')) {

        // build fullPath 
    
        string fullPath = dir + "/" + arg ; 

        // if path exists and is executablex
        
        if(fs::exists(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
            return fullPath ;
        }
        
    }

    return "" ; 
}



void handleTypeCommand(vector<string> &args){


    
     for(int i = 1 ; i < args.size() ; i++){

         string arg = args[i] ; 
         
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


void executeProgram(const string &fullPath , vector<string> &args) {

   

    vector<char*> argv ; 

    // argv accepts char* argv[] in C 

    for(auto &str : args) {
        argv.push_back(str.data());
    }

    // end of args 
    argv.push_back(nullptr);

    // create a child process 
    pid_t pid = fork() ;

    // inside child
    if(pid == 0) {

        // execute
        execv(fullPath.c_str() , argv.data());

        // if execution fails 
        perror("execv failed");
        exit(1);
    }
    // inside parent
    else if(pid > 0) {
        // wait until child finish
        waitpid(pid, nullptr , 0) ;
    }
     // pid < 0
    else {
        perror("fork failed");
    }
}


void printCurrentDir() {
    cout << fs::current_path().string() << endl ; 
}

void changeDirectory(vector<string> &args) {

    string dir ; 
        
   if(args.size() == 1) {
       dir = "empty";
   }
   else {
       dir = args[1] ; 
   }




    char* home = getenv("HOME");

    // special ~ -> home 
    if(dir == "~" || dir=="empty") {
        fs::current_path(string(home));
        return ;
    }


    if(fs::exists(dir) && fs::is_directory(dir)) {
       fs::current_path(dir) ;
   }
    else {
       cout << "cd" << ": " << dir << ": No such file or directory" << endl ;
   }
    
}