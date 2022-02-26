
/*
 * TIMER LED BLINK
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
*/
const int LED_A = 8;
const int LED_B = 7;
const int DELAY = 300;
const int DELTA = 30;


int period_A = 0;  
int period_B = 0;

int period_A_cmp = 50;
int period_B_cmp = 20*4/3;

bool toggle_status_A = 0;
bool toggle_status_B = 0;

void setup() {
  // Serial.begin(9600);  

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  
  //global_counter = 0;
  // initialize Timer1
  cli();             // disable global interrupts
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0;
  
  // Set CS10 and CS12 bits so timer runs at clock speed /1024: 
  // CSxx=1 0 1
  // TCCR1B |= (1 << CS12)|(1 << CS10); 

  // CSxx=1 0 0 | WGM12=CTC Mode
  // TCCR1B |= (1 << CS12)|(1 << WGM12); 

  // CSxx=0 0 1 | WGM12=CTC Mode
  TCCR1B |= (1 << CS10)|(1 << WGM12);  
 
  // enable Timer1 overflow interrupt:
  // TIMSK1 = (1 << TOIE1); // Timer overflow
  TIMSK1 = (1 << OCIE1A); // Output compare match A

  // OCR1A = 32767; 
  OCR1A = 1024; 
  // OCR1A = 32767;

  
  // enable global interrupts:
  sei();
}

void loop() {
  delay(1000); // delay uses timer0 
  // Serial.print("counter: ");
  // Serial.println(global_counter);
}

/*
ISR(TIMER1_OVF_vect)
{
  toggle_status = !toggle_status;
  digitalWrite(LED_A, toggle_status); 
  digitalWrite(LED_B, !toggle_status);   
}
*/ 

ISR(TIMER1_COMPA_vect)
{
  period_A > period_A_cmp ? 0 : period_A + 1;
  period_B > period_B_cmp ? 0 : period_B + 1;

  toggle_status_A = period_A > 0 ? toggle_status_A : !toggle_status_A;
  toggle_status_B = period_B > 0 ? toggle_status_B : !toggle_status_B;
    
  digitalWrite(LED_A, toggle_status_A); 
  digitalWrite(LED_B, toggle_status_B);  

  PORTD = PORTD | (toggle_status_A << 7);
  PORTD = PORTD | (toggle_status_B << 6);
   
}
