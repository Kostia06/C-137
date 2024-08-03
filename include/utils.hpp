#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#define RED string("\033[1;31m")
#define GREEN string("\033[1;32m")
#define PURPLE string("\033[1;35m")
#define WHITE string("\033[1;37m")
#define RESET string("\033[0m")

string readFile(string fileName);
vector<string> split(string str, string delimiter);
