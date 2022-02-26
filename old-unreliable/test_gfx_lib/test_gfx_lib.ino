#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
#include <SPI.h>


#define CS    10
#define DC     9
#define RESET  8
#define SCLK  13
#define MOSI  11

#define BORDER_PADDING  3

Adafruit_ST7735 tftDisplay = Adafruit_ST7735(CS, DC, RESET);
 
int x = 0;
int y = 0;
int xp = 0;
int yp = 0;

int x_min, x_max, y_min, y_max, y_offset, h_max;

void setup() {
  general_setup();
  set_display_limits();
  draw_screen();
}

void set_display_limits(){
  
  x_min = BORDER_PADDING;
  y_min = tftDisplay.height() - BORDER_PADDING;
  x_max = tftDisplay.width() - BORDER_PADDING; 
  y_max = BORDER_PADDING; 
  y_offset = (y_min + y_max) / 2;
  h_max =  (y_min + y_max) / 2 - 5 * BORDER_PADDING;

  Serial.println("x_min=" + String(x_min) );
  Serial.println("y_min=" + String(y_min) );
  Serial.println("x_max=" + String(x_max) );
  Serial.println("y_max=" + String(y_max) );
  Serial.println("y_offset=" + String(y_offset) );
  Serial.println("h_max=" + String(h_max) );


  
}

void general_setup() {
  Serial.begin (9600); // serial monitor stuff
  Serial.println ("TFT ST7735 display test");


  tftDisplay.initR (INITR_BLACKTAB);
  tftDisplay.setRotation(3);
  Serial.println("w=" + String(tftDisplay.width()) + "; h=" + String(tftDisplay.height()));
}

void draw_screen() {
  int padding = BORDER_PADDING;
  
  tftDisplay.fillScreen (ST7735_BLACK);

  int x0 = padding;
  int y0 = padding;
  int w = x_max - x_min;
  int h = y_min - y_max; // Or Adafruit will kill everybody
  tftDisplay.drawRect(x0, y0, w, h, ST7735_GREEN);

  tftDisplay.setCursor (80, 100);
  tftDisplay.print ("Hello");

}

void initPointHistory() {
  x = x_min;
  y = y_offset;
  xp = x_min;
  yp = y_offset;
}


void drawGraphPoint(Adafruit_ST7735 d, int xi, int yi) {
  x = xi;
  y = yi;
  d.drawLine(xp, yp, x, y, ST7735_GREEN);
  xp = xi;
  yp = yi;
}

void loop() {
  int xs = x - x_min;
  int ys = int(h_max * sin(9.0 * float(xs) / float(x_max - x_min) )  );
  y =  y_offset - ys;
  Serial.println( "x=" + String(x) + "; y=" + String(y) );
  drawGraphPoint(tftDisplay, x, y);
  
  x ++;
  if (x == x_max) {
    draw_screen();
    initPointHistory();
  }
  
  delay(10);

}
