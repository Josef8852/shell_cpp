#include "shell.h"




int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;

  std::cerr << std::unitbuf;

  Shell shell ; 

  shell.run();


}
