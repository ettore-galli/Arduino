/*
  Arduino Analog Monitor

  Main

*/

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
#include <SPI.h>

#ifndef layout_h
#define layout_h
#include "layout.h"
#endif

#ifndef tracer_h
#define tracer_h
#include "tracer.h"
#endif

#define CS    10
#define DC     9
#define RESET  8
#define SCLK  13
#define MOSI  11

Adafruit_ST7735 tftDisplay = Adafruit_ST7735(CS, DC, RESET);
ScreenLayout sl = ScreenLayout(tftDisplay);

PlotTracer pt = PlotTracer(sl);

int x = 0;
int y = 0;

void setup() {
  general_setup();
  set_display_limits();
  draw_screen();
  print_setup();
  pt.reset();
}


void general_setup() {
  Serial.begin (9600); // serial monitor stuff
  tftDisplay.initR (INITR_BLACKTAB);
  tftDisplay.setRotation(3);
}


void set_display_limits() {
  sl.initDisplay(tftDisplay);
}


void draw_screen() {
  int padding = BORDER_PADDING;

  tftDisplay.fillScreen (ST7735_BLACK);

  int x0 = padding;
  int y0 = padding;
  int w = sl.x_max - sl.x_min;
  int h = sl.y_min - sl.y_max; // Or Adafruit will kill everybody
  tftDisplay.drawRect(x0, y0, w, h, ST7735_GREEN);

  tftDisplay.setCursor (80, 100);
  tftDisplay.print ("Hello");

}



void loop() {
  x = pt.getCurrentPoint().x;

  int xs = x - sl.x_min;

  //int ys = int(sl.h_max * sin(9.0 * float(xs) / float(sl.x_max - sl.x_min) )  );
  int ys = int(sl.h_max * pow(sin(9.0 * float(xs) / float(sl.x_max - sl.x_min) ), 19) );
  y =  sl.y_offset - ys;


  // drawGraphPoint(tftDisplay, x, y);
  pt.drawPlotPoint(tftDisplay, x, y, ST7735_GREEN);

  pt.nextX();
  if (pt.isLimitReached()) {
    pt.reset();
    draw_screen();

  }

  delay(10);

}


void print_setup() {
  Serial.println("x_min=" + String(sl.x_min) );
  Serial.println("y_min=" + String(sl.y_min) );
  Serial.println("x_max=" + String(sl.x_max) );
  Serial.println("y_max=" + String(sl.y_max) );
  Serial.println("y_offset=" + String(sl.y_offset) );
  Serial.println("h_max=" + String(sl.h_max) );
}


