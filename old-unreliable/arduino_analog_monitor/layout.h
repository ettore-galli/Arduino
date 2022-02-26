/*
  Arduino Analog Monitor

  Screen Layout

*/
#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h>

#define BORDER_PADDING  3

class ScreenLayout {
  public:
    int width = 0;
    int height = 0;
    int x_min = 0; 
    int x_max = 0; 
    int y_min = 0; 
    int y_max = 0; 
    int y_offset = 0; 
    int h_max = 0;

    ScreenLayout (Adafruit_ST7735 tftDisplay);
    void initDisplay (Adafruit_ST7735 tftDisplay);

};
