#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
#include <SPI.h>

/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink

  1.1.1 :  08/10/2017 (galli)
           ModifiHato il tèmpo di blinh


*/

const int messaggio[] = {0,0,0,1,1,1,0,0,0};
const int unit = 100;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  int i;
  for (i = 0; i < sizeof(messaggio) / sizeof(int); i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay (unit + 2 * unit * messaggio[i]);
    digitalWrite(LED_BUILTIN, LOW);
    delay (unit);
  }
  
  delay (5 * unit);

}
