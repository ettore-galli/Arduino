/*
  Arduino Analog Monitor

  Plot tracer

*/

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h>

#ifndef tracer_h
#define tracer_h
#include "tracer.h"
#endif

#ifndef layout_h
#define layout_h
#include "layout.h"
#endif 

PlotTracer::PlotTracer (ScreenLayout& s) : sl{s} {
   sl = s;
}

void PlotTracer::drawPlotPoint(Adafruit_ST7735 d, int x, int y, int color){
  previous.x = current.x;
  previous.y = current.y;
  d.drawLine(previous.x, previous.y, x, y, color);
  d.drawPixel(x, y, ST7735_RED);
  current.x = x;
  current.y = y;
}

void PlotTracer::reset(){
  previous.x = sl.x_min;
  previous.y = sl.y_offset;
  current.x = sl.x_min;
  current.y = sl.y_offset;
}

bool PlotTracer::isLimitReached(){
  return current.x == sl.x_max;
}

void PlotTracer::nextX(){
  current.x ++;
}

Point PlotTracer::getCurrentPoint(){
  return current;
}

Point PlotTracer::getPreviousPoint(){
  return previous;
}





