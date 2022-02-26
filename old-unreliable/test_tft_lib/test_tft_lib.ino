#include <TFT.h> // Hardware-specific library
#include <SPI.h>


#define CS    10
#define DC     9
#define RESET  8
#define SCLK  13
#define MOSI  11

TFT tftDisplay = TFT(CS, DC, RESET);

int x = 0;
int y = 0;
int xp = 0;
int yp = 0;

void setup() {
  general_setup();
  draw_screen();
}

void general_setup() {
  tftDisplay.begin();
  tftDisplay.background(0, 0, 0);
  initPointHistory();
}

void draw_screen() {
  tftDisplay.stroke(0, 255, 0);
  for (int i = 1; i < 4; i++) {
    int padding = i;
    tftDisplay.rect(padding, padding, tftDisplay.width() - 1 - 2 * padding, tftDisplay.height() - 1 - 2 * padding);
  }

}

void initPointHistory(){
  x = 0;
  y = 0;
  xp = 0;
  yp = 0;
}


void drawGraphPoint(TFT d, int xi, int yi){
  x = xi;
  y = yi;
  d.line(xp, yp, x, y);
  xp = xi;
  yp = yi;
}

void loop() {
  y = (tftDisplay.height() / 2) + int((tftDisplay.height() / 2) * sin(9.0 * float(x) / float(tftDisplay.width())));
  drawGraphPoint(tftDisplay, x, y);
  x ++;
  if (x == tftDisplay.width()){
    initPointHistory();
  }
  delay(100);

}
