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
#include <algorithm>


using namespace  std ;

namespace fs = filesystem;

string getPath();

vector<string> parseLine(const string &input);

void printLine(vector<string> &args) ;

string findExecutableInPath(const string &path , const string &arg);

void handleTypeCommand(vector<string>&args);

void executeProgram(const string &fullPath , vector<string> &args);

void printCurrentDir() ;

void changeDirectory(vector<string> &args);