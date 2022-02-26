/*
  Arduino Analog Monitor

  Screen Layout

*/

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h>

#ifndef layout_h
#define layout_h
#include "layout.h"
#endif

ScreenLayout::ScreenLayout (Adafruit_ST7735 tftDisplay) {
  //initDisplay(tftDisplay);
}

void ScreenLayout::initDisplay (Adafruit_ST7735 tftDisplay) {

  
  width = tftDisplay.width();
  height = tftDisplay.height();

  x_min = BORDER_PADDING;
  y_min = tftDisplay.height() - BORDER_PADDING;
  x_max = tftDisplay.width() - BORDER_PADDING;
  y_max = BORDER_PADDING;
  
  y_offset = (y_min + y_max) / 2;
  h_max =  (y_min + y_max) / 2 - 5 * BORDER_PADDING;

}



