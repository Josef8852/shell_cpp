#include "shell.h"




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

      vector<string> args = parseLine(line);

      string command = args[0] ;


      if(command.empty()) continue;

      if(command == "exit") break ;

      else if(command == "echo") printLine(args);

      else if(command == "pwd") printCurrentDir() ;

      else if(command == "type")  handleTypeCommand(args);

      else if(command == "cd") changeDirectory(args);
      
      else {

          string fullPath = findExecutableInPath(getPath(), command) ;
          
          if(!fullPath.empty()) {

              executeProgram(fullPath, args) ;
          }
          else {
              cout << command << ": command not found" << endl ;
          }
      }

  }


}
