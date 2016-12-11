#ifndef __CLI_H_INCLUDED__
#define __CLI_H_INCLUDED__ 

using namespace std;

class CLI
{
  public:
    static string exec(const char* cmd);
    //vector<string> getNearByCells();
    static string convertInt(int number);
};

#endif
