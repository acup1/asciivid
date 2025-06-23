#include <atomic>
#include <cstdint>
#include <fcntl.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

void kb_read(atomic<uint8_t> &key) {
  struct termios attr;
  tcgetattr(STDIN_FILENO, &attr);
  attr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &attr);
  ssize_t bytes;
  while ((bytes = read(STDIN_FILENO, &key, 1)) > 0) {
  }
}
