#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <stdio.h>

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

