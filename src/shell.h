#pragma once 

#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace  std ;

string getPath();

void printLine(stringstream &params);

string findExecutableInPath(const string &path , const string &arg);

void handleTypeCommand(stringstream &params);

void executeProgram(const string &command , const string &fullPath ,  stringstream &params ) ;