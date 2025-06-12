#include <cmath>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

void buff(bool type) {
  if (not type)
    cout << u8"\033[?1049l";
  else
    cout << u8"\033[?1049h";
}

void cursor(bool visibility) {
  if (visibility)
    cout << u8"\033[?25h";
  else
    cout << u8"\033[?25l";
}

void clear() { cout << u8"\033[2J"; }

void pos_print(int x, int y, const string s) {
  cout << u8"\033[" << y + 1 << u8";" << x + 1 << u8"H" << s;
}

void get_term_size(int &x, int &y) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  x = w.ws_col;
  y = w.ws_row;
}
