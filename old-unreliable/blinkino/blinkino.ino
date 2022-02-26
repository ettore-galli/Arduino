

const int LED_A = 8;
const int LED_B = 14;
const int DELAY = 300;
const int DELTA = 30;

int count = 0;
int actualDelay = 0;
int delayDelta = DELTA;

// https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
// https://arduino.stackexchange.com/questions/12382/where-is-documentation-on-arduinos-internal-interrupts
// https://github.com/ElektorLabs/Arduino/blob/master/source2/avr/cores/arduino/avr/iom328p.h




void setupTimer(){
  TIMSK1 = 7;
}
 
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
}




// the loop function runs over and over again forever
void loop() {
  
  if (actualDelay <= 0){
    delayDelta = DELTA;
  }
  if (actualDelay >= DELAY){
    delayDelta = -DELTA;
  }
  
  digitalWrite(LED_A, HIGH);   
  digitalWrite(LED_B, LOW);
   
  delay(actualDelay);    
                    
  digitalWrite(LED_A, LOW);    
  digitalWrite(LED_B, HIGH);
  
  delay(actualDelay); 

  actualDelay += delayDelta;                      
}
