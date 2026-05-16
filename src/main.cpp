#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include "shell.h"

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

      else if(command == "type") {

          handleTypeCommand(params);


      }

      else {

          string fullPath = findExecutableInPath(getPath(), command) ;
          
          if(!fullPath.empty()) {

              string arg ; 

              vector<string> args ; 

              args.push_back(command) ;
              
              while(params >> arg) {
                  args.push_back(arg) ;
              }

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
                  execv(fullPath.c_str() , argv.data());
                  perror("execv failed");
                  exit(1);
              }
              // inside parent
              else if(pid > 0) {
                  // wait until child finish
                  waitpid(pid, nullptr , 0) ;
              }
              else {
                  perror("fork failed");
              }
          }
          else {
              cout << command << ": command not found" << endl ;
          }
      }




  }


}
