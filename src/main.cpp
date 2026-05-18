#include "shell.h"
#include <filesystem>



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

      else if(command == "pwd") printCurrentDir() ;

      else if(command == "type")  handleTypeCommand(params);

      else if(command == "cd") {

          string dir ;

          params >> dir;

          
          if(fs::exists(dir) && fs::is_directory(dir)) {
              fs::current_path(dir) ;
          }
          else {
              cout << command << ": " << dir << ": No such file or directory" << endl ;
          }
          
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
