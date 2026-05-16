#pragma once 

#include <string>
#include <sstream>

using namespace  std ;

string getPath();

void printLine(stringstream &params);

string findExecutableInPath(const string &path , const string &arg);

void handleTypeCommand(stringstream &params);