#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>

using namespace std ;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;

  cerr << unitbuf;

  string PS1 = "$ " ;


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
          string rest ;
          getline(params , rest);

          if(!rest.empty() && rest[0] == ' ') {
              rest.erase(0,1);
          }

          cout << rest << endl ;
      }
      else if(command == "type") {
          
          string arg ;

          while(params >> arg) {
              if(builtins.count(arg)) {
                  cout << arg << " is a shell builtin" << endl ;
              }
              else {
                  cout << arg << ": not found" << endl ;
              }
          }
      }
      else {
          cout << command << ": command not found" << endl ;
      }




  }


}
