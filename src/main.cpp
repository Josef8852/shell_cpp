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


      if(command == "exit") break ; 
      

         
      cout << command << ": command not found" << endl ;

      
  }


}
