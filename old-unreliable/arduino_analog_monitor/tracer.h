/*
  Arduino Analog Monitor

  Plot tracer

*/

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h>

#ifndef layout_h
#define layout_h
#include "layout.h"
#endif

struct Point {
  unsigned char x;
  unsigned char y;
};

class PlotTracer {

  private:
    Point previous, current;

  public:
    ScreenLayout& sl;
    PlotTracer(ScreenLayout& s);
    void drawPlotPoint(Adafruit_ST7735 d, int x, int y, int color);
    void reset();
    bool isLimitReached();
    void nextX();
    Point getCurrentPoint();
    Point getPreviousPoint();

};



