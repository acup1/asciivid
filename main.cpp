#include "func.cpp"
#include "func.h"
#include <clocale>
#include <cmath>
#include <ctime>
// #include <iostream>
// #include <locale>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

vector<string> charset{u8" ", u8"░", u8"▒", u8"▓", u8"█"};
const int charsetlen = 5;

int main(int argc, char *argv[]) {
  // const char *name = "2.mp4";
  const char *name = argv[1];
  // wcout.imbue(locale(""));
  int x, y;
  float pixel;
  get_term_size(x, y);

  //  cout << x << ' ' << y << '\n';
  //  for (int i = 0; i < charsetlen; i++) {
  //    cout << charset[i] << '\n';
  //  }
  //  cout << u8"█\n";
  //
  //  system("read");

  buff(1);
  cursor(0);

  VideoCapture cap(name);
  if (!cap.isOpened())
    return -1;

  double fps = cap.get(CAP_PROP_FPS);
  int frameCount = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));
  Mat frame;

  time_t t1;
  while (true) {
    t1 = time(nullptr);
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
        // float pixel = row_ptr[col];
        pos_print(col, row, charset[(int)pixel]);
        // cout << pixel << '\n';
      }
    }

    while ((time(nullptr) - t1) < (1 / 24)) {
      {
        ;
      }
    }
  }
  cap.release();

  buff(0);
  cursor(1);
  return 0;
}
