
/*
 * TIMER TOY ORGAN
 * 
 * Headers
 * https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h
 * 
 * Stackexchange "Where is documentation?"
 * https://arduino.stackexchange.com/questions/12382/where-is-documentation-on-arduinos-internal-interrupts
 * 
 * Timer tutorial
 * https://github.com/ElektorLabs/Arduino/blob/master/source2/avr/cores/arduino/avr/iom328p.h
 * 
 * Interrupts
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 * 
 * Datasheet
 * https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
 * 
 * Timer Interrupt Setup Example
 * https://forum.arduino.cc/t/arduin-o-scope-a-tiny-oscilloscope-project/8138
 * 
*/

#define BAUD_RATE 115200 

const int LED_A = 6;
const int LED_B = 7;
const int OC1A = 9;
const int OC1B = 10;

const int DELAY = 300;
const int DELTA = 30;


volatile int period_A = 0;  
volatile int period_B = 0;

volatile int period_A_cmp = 128;
volatile int period_B_cmp = 128;

volatile int toggle_status_A = 0;
volatile int toggle_status_B = 0;

void setup() {

  Serial.begin(BAUD_RATE); 

  // Initialize Output pins
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(OC1A, OUTPUT);
  pinMode(OC1B, OUTPUT);
  
  // initialize Timer1
  cli();             // disable global interrupts

  // set entire TCCR1A/B registers to 0
  TCCR1A = 0;        
  TCCR1B = 0;        

  // Enable CTC mode
  TCCR1B |= (1 << WGM12);

  // "Clear OC1A/OC1B on Compare Match when upcounting.
  // Set OC1A/OC1B on Compare Match when downcounting."
  TCCR1A |= (0 << COM1A1) | (1 << COM1A0);

  
  // Set prescaler
  TCCR1B |= (1 << CS10);
  TCCR1B |= (0 << CS11);
  TCCR1B |= (0 << CS12);

  // Enable Output Compare match
  // TIMSK1 = (1 << OCIE1A);

  // Set Output compare value (max is 32767)
  OCR1A = 1000; 

  // enable global interrupts:
  sei();
  
}

void loop() {
}

ISR(TIMER1_COMPA_vect)
{
 /*
  
  period_A = period_A > period_A_cmp ? 0 : period_A + 1;
  period_B = period_B > period_B_cmp ? 0 : period_B + 1;
  
  toggle_status_A = period_A ? toggle_status_A : !toggle_status_A;
  toggle_status_B = period_B ? toggle_status_B : !toggle_status_B;

  // if (!period_A) toggle_status_A = !toggle_status_A;
  // if (!period_B) toggle_status_B = !toggle_status_B;
    
  PORTD = (toggle_status_A << 6) | (toggle_status_B << 7);
 
  //Serial.println(micros());
*/
  
}
