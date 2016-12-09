#include <iostream>
#include <curses.h>

using namespace std;

int main ()
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int i;
  cout << "Please enter an integer value: ";
  cin >> i;
  cout << "The value you entered is " << i;
  cout << " and its double is " << i*2 << ".\n";

  endwin()
  return 0;
}
