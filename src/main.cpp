#include <iostream>
#include <string>

using namespace std ;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;

  cerr << unitbuf;

  string PS1 = "$ " ;


  
  //REPL
  while(true) {

      cout << PS1 ;
      
      string command ;
    
      getline(cin , command) ;

   

      if(command == "exit") break; 
      else if(command.substr(0,5) == "echo ") {
          cout << command.substr(5) << endl ; 
      }
      else if(command.substr(0,5) == "type ") {

          if(command.substr(5) == "type" || command.substr(5) == "exit" || command.substr(5) == "echo")  {
              cout << command.substr(5) << " is a shell builtin" << endl ; 
          }
          else {
              cout << command.substr(5) << ": not found" << endl ; 
          }
          
      }
      else {
          cout << command << ": command not found" << endl ;
      }
      


      
  }

  
}
