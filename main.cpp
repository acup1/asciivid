#include "func.cpp"
#include "func.h"
#include "kb.cpp"
#include "kb.h"
#include <atomic>
#include <clocale>
#include <cmath>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <signal.h>
#include <stdint.h>
#include <string>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace cv;

void sigint_handler(int sig);
void kb_read(atomic<uint8_t> &key);

atomic<bool> running(true);
vector<string> charset{" ", "░", "▒", "▓", "█"};
const int charsetlen = charset.size();

int main(int argc, char *argv[]) {
  signal(SIGINT, sigint_handler);
  atomic<uint8_t> key;
  atomic<bool> pause(false);
  thread kb_thread(kb_read, ref(key));

  const char *name = argv[1];
  int x, y;
  get_term_size(x, y);

  buff(1);
  cursor(0);

  VideoCapture cap(name);
  if (!cap.isOpened())
    return -1;

  double fps = cap.get(CAP_PROP_FPS);
  double targetFrameTime = CLOCKS_PER_SEC / fps;
  int frameCount = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));
  Mat frame;

  double t1;
  while (running) {

    while (running) {
      t1 = clock();
      get_term_size(x, y);
      cap >> frame;

      if (frame.empty())
        break;

      resize(frame, frame, Size(x, y), 0, 0, INTER_LINEAR);
      cvtColor(frame, frame, COLOR_BGR2GRAY);
      frame.convertTo(frame, CV_32F);
      normalize(frame, frame, 0, 255, NORM_MINMAX);

      for (int row = 0; row < frame.rows; row++) {
        float *row_ptr = frame.ptr<float>(row);
        for (int col = 0; col < frame.cols; col++) {
          float pixel = round(row_ptr[col] / 255 * (charsetlen - 1));
          pos_print(col, row, charset[(int)pixel]);
        }
      }

      while (((clock() - t1) < targetFrameTime) or pause) {
        switch (key) {
        case ' ':
          pause = 1 - pause;
          break;
        case 'a':
          cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) - 10);
          break;
        case 'A':
          cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) - 20);
          break;
        case 'd':
          cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) + 10);
          break;
        case 'D':
          cap.set(CAP_PROP_POS_FRAMES, cap.get(CAP_PROP_POS_FRAMES) + 20);
          break;
        case 'q':
          running = false;
          break;
        }
        key = 0;
      }
    }
    cap.set(CAP_PROP_POS_FRAMES, 0);
  }
  cap.release();

  buff(0);
  cursor(1);
  return 0;
}

void sigint_handler(int sig) { running = false; }
