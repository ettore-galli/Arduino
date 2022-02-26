// SPI Adafruit ST7735 compatible
// TFT_160x128_ST7735
//
// 1.8″ 160×128 TFT Color Display
//
// Floris Wouterlood
// public domain August 2, 2017
// this is a bare sketch
// purpose: connect and start working

#define sclk_pin 13
#define mosi_pin 11
#define cs_pin 10
#define dc_pin 9
#define rst_pin 8

#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
#include <SPI.h>

 

// Option 1: use any pins but a little slower
Adafruit_ST7735 tft = Adafruit_ST7735 (cs_pin, dc_pin, rst_pin);

void setup() {

  Serial.begin (9600); // serial monitor stuff
  Serial.println ("TFT ST7735 display test");
  Serial.println ();
  
  tft.initR (INITR_BLACKTAB); // initialize a ST7735S chip, black tab
  Serial.println ("TFT initialized");
  
  // prepare the TFT
  tft.setTextColor (ST7735_WHITE);
  tft.fillScreen (ST7735_RED);
  
  tft.drawRect (5,5,119,60,ST7735_YELLOW); // upper rectangle
  tft.drawRect (5,70,119,84,ST7735_YELLOW); // lower reactangle
  
  tft.setCursor (30, 15); // put text in upper rectangle
  tft.setTextSize (2); // select text size
  tft.println ("ST7735");
  tft.setCursor (30, 30);
  tft.println ("======");
  tft.setTextSize (1); // select text size
  tft.setCursor (22, 45);
  tft.println ("160×128 pixels");
  tft.setTextSize(2); // set text size back to original

}

void loop() {

  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor (37, 90);
  tft.print ("Ha, die");
  delay (500);
  tft.fillRect (37, 90, 60, 20, ST7735_RED); // clear "Hello"
  tft.setCursor (37, 120);
  tft.print ("Henk");
  delay(500);
  tft.fillRect (37, 120, 60, 20, ST7735_RED); // clear "World"

}
