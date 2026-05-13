#include <iostream>
#include <string>

using namespace std ;

int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  
  cerr << unitbuf;


  string command ; 

  getline(cin , command) ;


  cout << command << ": command not found" << endl ; 
  
  
  cout << "$ ";
}
