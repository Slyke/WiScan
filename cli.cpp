#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <sstream>

#include "cli.h"

using namespace std;

string CLI::exec(const char* cmd) {
  char buffer[128];
  string result = "";
  FILE* pipe = popen(cmd, "r");
  if (!pipe) throw std::runtime_error("popen() failed!");
  try {
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
  } catch (...) {
    pclose(pipe);
    throw;
  }
  pclose(pipe);
  return result;
}

string CLI::convertInt(int number) {
   stringstream ss;
   ss << number;
   return ss.str();
}

/*
vector<string> CLI::getNearByCells() {
  char input[16] = "          Cell ";
  char *token = std::strtok(input, ":");
  std::vector<int> v;
  while (token != NULL) {
    v.push_back( std::strtol( token, NULL, 10 ));
    token = std::strtok(NULL, ":");
  }

  return v;

}
*/


