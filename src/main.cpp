#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>

using namespace std ;

namespace fs = filesystem;



unordered_set<string> builtins = {
    "echo" ,
    "exit" ,
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



void handleTypeCommand(const string &line){

        string arg ;

        string path = getPath();

        stringstream params(line) ;
    
    while(params >> arg) {
        if(builtins.count(arg)) {
            cout << arg << " is a shell builtin" << endl ;
        }
        else {


            string fullPath = findExecutableInPath(path , arg);
            
            if(!fullPath.empty()) {
                cout << arg << " is " << fullPath << endl ; 
         
            }
            else {
               cout << arg << ": not found" << endl ; 
            }
            
        }
    }
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;

  cerr << unitbuf;

  string PS1 = "$ " ;
  

  //REPL             
  while(true) {

      cout << PS1 ;

      string line ;

      getline(cin , line) ;


      stringstream params(line);


      string command ;

      params >> command ;

      if(command.empty()) continue; 

      if(command == "exit") break ; 

      else if(command == "echo") printLine(params);
      
      else if(command == "type") {
          
          handleTypeCommand(line);
       
      }
      
      else {
          cout << command << ": command not found" << endl ;
      }




  }


}
