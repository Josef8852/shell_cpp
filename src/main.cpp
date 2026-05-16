#include "shell.h"
#include <filesystem>

using namespace std;


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

      else if(command == "pwd") {
          cout << filesystem::current_path().string() << endl ; 
      }

      else if(command == "type") {

          handleTypeCommand(params);


      }

      else {

          string fullPath = findExecutableInPath(getPath(), command) ;
          
          if(!fullPath.empty()) {

              executeProgram(fullPath, command , params) ;
          }
          else {
              cout << command << ": command not found" << endl ;
          }
      }




  }


}
