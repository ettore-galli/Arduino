#include <CapacitiveSensor.h>


// PITCH CONTROL INPUT
// Pin setting
const int SENSOR_PORT = A1;
// Range values and threshold
int port_range_min = 1023;
int port_range_max = 0;
int port_threshold = 0;
// Duration of the setup lop procedure
const int SETUP_LOOP_DURATION_MILLIS = 5000;

// Calibration loop indicator
const int LED_PORT = 3;

// PWM SETTINGS
// PWM Port
const int INT_1A_PORT = 9;
const int INT_2A_PORT = 11;
// PWM Phase increment range
// Since it's a theremin, we don't actually need an exact "frequency" 
// but a range of phase increments per each interrupt, and a range of frequencies.
// System clock is 16 MHz, interrupt setting is for phase correct pwm in 510
// steps, so interrupt frequency is fint = 16.0Mhz / 510 counts/interrupt = 31372.54 interrupts/s 
// The sine wave table wave_len = 256 samples long, so for a phase shift of one for each clock
// interrupt the ouput frequency would be fint/wave_len = 122.54 Hz
// So, roughly, with a phase shift of 0.25 we get a ~30 Hz wave, 2.5=~300Hz, 25=~3000Hz that is quite 
// the maximum if we want to have at least 10 pwm cycles per wave period.
// Actual values below come from experimentation and optimization
const float PHASE_SHIFT_MIN = .1;
const float PHASE_SHIFT_MAX = 15.0; // 15

// DEFINITION OF THE WAVETABLE 
const int WAVETABLE_LEN = 256;
short sine[WAVETABLE_LEN] = {
  128,  131,  134,  137,  140,  143,  146,  149,  153,  156,  159,  162,  165,  168,  171,  174,  
  177,  180,  182,  185,  188,  191,  194,  196,  199,  201,  204,  207,  209,  211,  214,  216,  
  218,  220,  223,  225,  227,  229,  231,  232,  234,  236,  238,  239,  241,  242,  243,  245,  
  246,  247,  248,  249,  250,  251,  252,  253,  253,  254,  254,  255,  255,  255,  255,  255,  
  255,  255,  255,  255,  255,  254,  254,  253,  253,  252,  251,  251,  250,  249,  248,  247,  
  245,  244,  243,  241,  240,  238,  237,  235,  233,  232,  230,  228,  226,  224,  222,  219,  
  217,  215,  213,  210,  208,  205,  203,  200,  198,  195,  192,  189,  187,  184,  181,  178,  
  175,  172,  169,  166,  163,  160,  157,  154,  151,  148,  145,  142,  139,  135,  132,  129,  
  126,  123,  120,  116,  113,  110,  107,  104,  101,  98, 95, 92, 89, 86, 83, 80, 77, 74, 71, 
  68, 66, 63, 60, 57, 55, 52, 50, 47, 45, 42, 40, 38, 36, 33, 31, 29, 27, 25, 23, 22, 20, 18, 17, 
  15, 14, 12, 11, 10, 8,  7,  6,  5,  4,  4,  3,  2,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  
  0,  0,  1,  1,  2,  2,  3,  4,  5,  6,  7,  8,  9,  10, 12, 13, 14, 16, 17, 19, 21, 23, 24, 26, 
  28, 30, 32, 35, 37, 39, 41, 44, 46, 48, 51, 54, 56, 59, 61, 64, 67, 70, 73, 75, 78, 81, 84, 87, 
  90, 93, 96, 99, 102,  106,  109,  112,  115,  118,  121,  124,  128
};  
short* wave = sine; // If i wanted to output a square wave i'd use an astable multivib...

// CAPACITIVE SENSOR
CapacitiveSensor   cs42 = CapacitiveSensor(4,2);

// ULTRASONIC SENSOR
const int ULTRASONIC_TRIG_PIN = 11; 
const int ULTRASONIC_ECHO_PIN = 12;


// RUNNING VARIABLES
// Actual phase 
float wave_phase_A = 0; // int is position on array
// Actual phase increment
float delta_A = 0.0;
// Volume of wave a
float volume_a = 1.0; // 0..1 range
// Delay between one measurement and another
const int PLAY_LOOP_DELAY = 5;
// Sensor value
float pitch_sensor_value = 0;

/*
 * Input/Outpurt ports setup
 */
void portSetup(){
  // Ultrasonic input
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT); 
  pinMode(ULTRASONIC_ECHO_PIN, INPUT); 
  // Pitch sensor input
  pinMode(SENSOR_PORT, INPUT); 
  // Calibration indicator LED
  pinMode(LED_PORT, OUTPUT);  
  // Interrupt 1A PWM output port
  pinMode(INT_1A_PORT, OUTPUT);  // AUDIO INTERRUPT 1 OUT
  pinMode(INT_2A_PORT, OUTPUT);  // PWM 2
}



/**
 * Interrupt setup 
 */
void timerInterruptSetup(){

  // Disable interrupts for the setup procedure
  noInterrupts();

  // TIMER 1 - AUDIO PWM OUTPUT
  
  // Disable everything
  TCCR1A = 0x00; // No clock source (Timer/Counter stopped) 
  TCCR1B = 0x00; // No clock source (Timer/Counter stopped)
   
  // Set clock prescaler to 1. For PWM mode that does not mean
  // the interrupt will be run once every clock, but once every
  // clock count according to the chosen resolution (WGM1x)
  //
  // Prescaler
  // Divider    CS12 CS11 CS10
  // 1             0    0    1
  // 256           1    0    0 
  // 1024          1    0    1     
  TCCR1B |= (0<<CS12) | (0<<CS11) | (1<<CS10); // 1

  // Set PWM mode to phase-correct PWM
  TCCR1A |= (0<<WGM11) | (1<<WGM10) ; // PWM 8 bits fclock/510
  TCCR1B |= (0<<WGM12); 

  // Enable output pin toggle
  TCCR1A |= (1<<COM1A1); // Clear OC1 on compare match
  
  // Set interrupt type to timer Overflow interrupt
  TIMSK1 = 0x00;
  TIMSK1 |= (1<<TOIE1); 

  // With the above settings, an output to OCR1A and OCR1B registers
  // will produce the PWM output on the respective pins.
  // Pin mapping
  // OCR1A for PWM output on pin 9
  // OCR1B for PWM output on pin 10

  
  // TIMER 2 - 
  //TODO: Cleanup 
  /*
  // Disable everything
  TCCR2A = 0x00; // No clock source (Timer/Counter stopped) 
  TCCR2B = 0x00; // No clock source (Timer/Counter stopped)
   
  // Set clock prescaler to ????. For PWM mode that does not mean
  // the interrupt will be run once every clock, but once every
  // clock count according to the chosen resolution (WGM1x)
  //
  // Prescaler
  // Divider    CS22 CS21 CS20
  // 1             0    0    1
  // 256           1    0    0 
  // 1024          1    1    1     
  //TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20); // 1024
  TCCR2B |= (0<<CS22) | (0<<CS21) | (1<<CS20); // No prescaling (divider = 1)

  // Set PWM mode to fast PWM mode
  TCCR2A |= (0<<WGM21) | (1<<WGM20) ; // PWM 8 bits fclock/510
  TCCR2B |= (1<<WGM22); 

  // Enable output pin toggle
  TCCR2A |= (1<<COM2A0); // Enable pin toggle on compare match 
  
  // Set interrupt type to timer Overflow interrupt
  TIMSK2 = 0x00;
  TIMSK2 |= (1<<TOIE2); 

  // With the above settings, an output to OCR2A and OCR2B registers
  // will produce the PWM output on the respective pins.
  // Pin mapping
  // OCR2A for PWM output on pin 11
  // OCR2B for PWM output on pin 3
  OCR2A = 32; // f = fclock/prescaler/(OCR2A) --> pin 11
  */
  
  // Restore interrupt functionality
  interrupts();  
}


 /**
  * Interrupt service routine for interrupt 1 - AUDIO PWM
  */
 ISR(TIMER1_OVF_vect) {   
  // The value of the phase shoft for each cycle is the global variable delta_A
  // Increment the phase by the increment
  wave_phase_A +=  delta_A;
  // Wrap around if it overflows
  if (wave_phase_A > WAVETABLE_LEN){
    wave_phase_A = wave_phase_A - WAVETABLE_LEN;
  }
  // Get the value from the table and output it
  OCR1A = (short) wave[((short) wave_phase_A)]*volume_a;
 }  

 
/**
 * Demo/Test pitch sweep
 */
void pichSweepDemo(){
  while (true){
    int i = 0;
    for (i=0; i<100; i++){
      delta_A = PHASE_SHIFT_MIN + i*(PHASE_SHIFT_MAX - PHASE_SHIFT_MIN)/100.0;
      delay(100);
    }
  }  
}


 /**           
  * Main setup procedure
  */
void setup() {
  Serial.begin(9600);
  portSetup();
  timerInterruptSetup();
}


float sr05sense(){
  /* ULTRASONIC DISTANCE */
  float duration;

  digitalWrite(ULTRASONIC_TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

  //Serial.println(duration);
  return duration;
 
}


/**
 * Main line 
 */
void loop() {
  const float min_duration = 100;
  const float max_duration = 2000;
  //const float w1 = 0.1;
  //const float w2 = 0.9;
  const float s = 1.0;
  // LIGHT SENSOR  
  float sr05 = 0;
  int NMIS = 10;
  int i;
  for (i=0; i< NMIS; i++){
      sr05 += sr05sense();
  }
  sr05 = sr05/NMIS;
  pitch_sensor_value =sr05;// w1*pitch_sensor_value + w2*sr05;
  
  //pitch_sensor_value = 300;
  //Serial.println(pitch_sensor_value);
  // VERSIONE SETUP AUTOMATICO
  if ((pitch_sensor_value > min_duration) && (pitch_sensor_value < max_duration)){
      delta_A = PHASE_SHIFT_MIN + pitch_sensor_value * s * (PHASE_SHIFT_MAX - PHASE_SHIFT_MIN) / (max_duration - min_duration);
  } else {
      delta_A = 0.0;
  }

  //delayMicroseconds(1000*PLAY_LOOP_DELAY); 
  //delay(PLAY_LOOP_DELAY);
  delay(10);

}



/*
void loop() {



  // LIGHT SENSOR  
  int sensor = sense();

  // VERSIONE SETUP AUTOMATICO
  if (sensor > port_threshold){
      delta_A = PHASE_SHIFT_MIN + sensor * (PHASE_SHIFT_MAX - PHASE_SHIFT_MIN) / (port_range_max - port_range_min);
  } else {
      delta_A = 0.0;
  }

    
  delay(PLAY_LOOP_DELAY);

  // pichSweepDemo();
}
*/
