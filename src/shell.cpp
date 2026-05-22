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



void printLine(stringstream &params) {
    string line;
    getline(params, line);

    if (!line.empty() && line[0] == ' ')
        line.erase(0, 1);

    bool isInsideSingleQuote = false;
    string word = "";
    vector<string> words;

    for (int i = 0; i < line.length(); i++) {

        char curr = line[i];

        if (curr == '\'') {
            isInsideSingleQuote = !isInsideSingleQuote;
            continue;
        }

        if (!isInsideSingleQuote && curr == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += curr;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    for (auto &word : words) {
        cout << word << " ";
    }

    cout << endl;
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


void executeProgram(const string &fullPath , const string &command , stringstream &params) {

    string arg ; 

    vector<string> args ; 

    args.push_back(command) ;
    
    while(params >> arg) {
        args.push_back(arg) ;
    }

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

void changeDirectory(stringstream &params) {
    string dir ;

    params >> dir;


    char* home = getenv("HOME");

    // special ~ -> home 
    if(dir == "~" || dir.empty()) {
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