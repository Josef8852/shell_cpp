#include "shell.h"


using namespace std ;

namespace fs = filesystem;



Shell::Shell() {}


void Shell::run() {
    
    while(true) {
  
        cout << PS1 ;
  
        string line ;
  
        getline(cin , line) ;
  
        vector<string> args = parseLine(line);

        if(args.empty()) continue;

        string command = args[0] ;
  
        if(command == "exit") break ;
  
        else if(command == "echo") printLine(args);
  
        else if(command == "pwd") printCurrentDir() ;
  
        else if(command == "type")  handleTypeCommand(args);
  
        else if(command == "cd") changeDirectory(args);
        
        else {
  
            string fullPath = findExecutableInPath(PATH, command) ;
            
            if(!fullPath.empty()) {
  
                executeProgram(fullPath, args) ;
            }
            else {
                cout << command << ": command not found" << endl ;
            }
        }
  
    }
}





string Shell::getPath() {
    char* p = getenv("PATH");
    return p ? string(p) : "";
}



vector<string> Shell::parseLine(const string &input) {
    vector<string> words;
    string word;
    bool isInsideSingleQuote = false , isInsideDoubleQuotes = false , isEscaping = false;

    for (char c : input) {

        if(isEscaping) {
            word += c ; 
            isEscaping = false ;
            continue ; 
        }

        if(c == '\\' && !isInsideSingleQuote) {
            isEscaping = true ; 
            continue ; 
        }

        if (c == '\'' && !isInsideDoubleQuotes) {
            isInsideSingleQuote = !isInsideSingleQuote;
            continue;
        }

        if( c == '\"' && !isInsideSingleQuote) {
            isInsideDoubleQuotes = !isInsideDoubleQuotes;
            continue;
        }

        if (c == ' ' && !isInsideSingleQuote && !isInsideDoubleQuotes) {
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


void Shell::printLine(vector<string> &args) {

    for(size_t i = 1 ; i < args.size() ; i++) {
        if(i > 1) cout << " ";
        cout << args[i] ;
    }

    cout << endl ; 
    
}


string Shell::findExecutableInPath(const string &path , const string &arg) {

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



void Shell::handleTypeCommand(vector<string> &args){


    
     for(size_t i = 1 ; i < args.size() ; i++){

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


void Shell::executeProgram(const string &fullPath , vector<string> &args) {

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


void Shell::printCurrentDir() {
    cout << fs::current_path().string() << endl ; 
}

void Shell::changeDirectory(vector<string> &args) {

  

    char* home = getenv("HOME");

    if(!home) {
        cout << "cd: Home not set" << endl ;
        return  ;
    }

    // special ~ -> home 
    if(args.size() == 1 || args[1] == "~") {
        fs::current_path(string(home));
        return ;
    }

    string dir = args[1];
    
    if(fs::exists(dir) && fs::is_directory(dir)) {
       fs::current_path(dir) ;
   }
    else {
       cout << "cd" << ": " << dir << ": No such file or directory" << endl ;
   }
    
}