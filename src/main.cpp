#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>

using namespace std ;

namespace fs = filesystem;


void printLine(stringstream &params) {
    string rest ;
    getline(params , rest);

    if(!rest.empty() && rest[0] == ' ') {
        rest.erase(0,1);
    }

    cout << rest << endl ;
}

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;

  cerr << unitbuf;

  string PS1 = "$ " ;

  char* path = getenv("PATH");

  string PATH = path ;

  
  unordered_set<string> builtins = {
      "echo" ,
      "exit" ,
      "type"
  };


  //REPL
  while(true) {

      cout << PS1 ;

      string line ;

      getline(cin , line) ;


      stringstream params(line);


      string command ;

      params >> command ;

      if(command == "exit") {
          break ;
      }

      if(command == "echo") {

        printLine(params);
      }

      
      else if(command == "type") {
          
          string arg ;

          stringstream dirs(PATH) ;

          while(params >> arg) {
              if(builtins.count(arg)) {
                  cout << arg << " is a shell builtin" << endl ;
              }
              else {

                  string dir ; 

                  bool found = false ; 

                  while(getline(dirs, dir, ':')) {

                      // build fullPath 
                      string fullPath = dir + "/" + arg ; 


                      // if path exists and is executable
                      
                      if(fs::exists(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
                          cout << arg << " is " << fullPath << endl ; 
                          found = true ;
                          break ; 
                      }
                      
                  }

                  if(!found) {
                      cout << arg << ": not found" << endl ; 
                  }
                  
              }
          }
      }
      else {
          cout << command << ": command not found" << endl ;
      }




  }


}
