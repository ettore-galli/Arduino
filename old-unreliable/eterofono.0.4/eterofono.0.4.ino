
// SENSORE
const int SENSOR_PORT = A0;
const int LED_PORT = 3;
const int PIEZO_PORT = 8;
const int INT_1A_PORT = 9;
const int SETUP_LOOP_DURATION_MILLIS = 5000;

const int PITCH_RANGE_LOW = 100;
const int PITCH_RANGE_HIGH = 2000;
const int PLAY_LOOP_DELAY = 10;
const int PITCH_DURATION = 15;

int port_range_min = 1023;
int port_range_max = 0;
int port_threshold = 0;

const float SYSTEM_CLOCK_FREQUENCY = 16e6;

const int BASE_SQUARE_PERIOD_MICROSECONDS = 33;
/*
const short WAVETABLE_LEN = 16;
short sine[WAVETABLE_LEN] = {128, 176, 218, 246, 256, 246, 218, 176, 128, 79, 37, 9, 0, 9, 37, 79};
*/


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
 


short* wave = sine;
float wave_frequency_A = 400.0;
float wave_phase_A = 0; // int is position on array
float delta_A = 5.0;
float interrupt_1_frequency = 0.0;


int led_status = HIGH;

void dumpClockSettings(){
  Serial.println("TCCR1A: ");
  Serial.println(TCCR1A, BIN);
  Serial.println(TCCR1B, BIN);
  Serial.println(CS10, BIN);
  Serial.println(CS11, BIN);
  Serial.println(CS12, BIN);


  /**
  TCCR2B = 0x00; // No clock source (Timer/Counter stopped) 
  TCNT2 = TCNT2init;
  TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20); //1024?
  TIMSK2 |= (1<<TOIE2); 
  */

  
  Serial.println("----------------");
}

/**
 * Returns the fraction of the whole period for each interrupt step; 
 * Returns a float fraction of nsamples.
 */
float delta_phase(float fwave, float finterrupt, int nsamples){
  return (fwave / finterrupt) * nsamples;
}


void timerInterruptSetup(){
  noInterrupts();
  TCCR1A = 0x00; // No clock source (Timer/Counter stopped) 
  TCCR1B = 0x00; // No clock source (Timer/Counter stopped)
   


  /**
   *   
   TCCR1B = (TCCR1B & 0b11100000) | 0b00001;

   TCCR1A = (TCCR1A &0b00001100)| 0b10100001;


   */

  // CS10=1, CS11=0, CS12=1 ==> Prescaler 1024
  int CS12SET=0;
  int CS11SET=0;
  int CS10SET=1;
  TCCR1B |= (CS12SET<<CS12) | (CS11SET<<CS11) | (CS10SET<<CS10); // 101=Prescaler 1024; 100=Prescaler 256; 001=Prescaler 1

  TCCR1B |= (0<<CS12) | (0<<CS11) | (1<<CS10); // 101=Prescaler 1024; 100=Prescaler 256; 001=Prescaler 1
  //TCCR1B |= (1<<CS12) | (0<<CS11) | (0<<CS10); // 101=Prescaler 1024; 100=Prescaler 256

  // Interrupt Expected frequency
  interrupt_1_frequency = SYSTEM_CLOCK_FREQUENCY / 510.0;

  // WGM11, WGM10, WGM12 = 1 ==> Fast PWM, Top = OCRA
  // COM1A0 = 1 ==> Enable pin toggle
  TCCR1A |= (0<<WGM11) | (1<<WGM10) ; // PWM 8 bits fclock/510
  //TCCR1A |= (1<<WGM11) | (0<<WGM10) ; // PWM 9 bits fclock/1022
  TCCR1A |= (1<<COM1A1);
  TCCR1B |= (0<<WGM12); 
  
  // Set interrupt type to timer Overflow interrupt
  TIMSK1 = 0x00;
  TIMSK1 |= (1<<TOIE1); 

  
  // Pin mapping
  // OCR1A = pin 9
  // OCR1B = pin 10
  interrupts();  
}

void portSetup(){
  
  // put your setup code here, to run once:
  //pinMode(SENSOR_PORT, INPUT); // Photoresistor
  //pinMode(LED_PORT, OUTPUT);   // Calibration indicator LED
  // pinMode(PIEZO_PORT, OUTPUT);   // PIEZO
  pinMode(INT_1A_PORT, OUTPUT);  // AUDIO INTERRUPT 1 OUT

  
}

 /**
  * Interrupt service routine for interrupt 1
  */
 ISR(TIMER1_OVF_vect) {   
  // Output wave A (pin 9)
  //delta_phase(wave_frequency_A, interrupt_1_frequency, WAVETABLE_LEN);
  //float delta = 2.08896*wave_frequency_A;
  wave_phase_A +=  delta_A;
  if (wave_phase_A > WAVETABLE_LEN){
    wave_phase_A = wave_phase_A - WAVETABLE_LEN;
  }
  //int a = int(wave_phase_A);
  //int b = (int(wave_phase_A) + 1) % WAVETABLE_LEN;
  
  //OCR1A =  int(128 + 0.3*(wave[b] + wave[a])/2.0);
  //OCR1A =  int(128 + 50*sin(6.28*wave_phase_A/WAVETABLE_LEN));
  //Serial.println(OCR1A);

  OCR1A = (short) wave[((short) wave_phase_A) % WAVETABLE_LEN]/4;
 }  
            
void setup() {
  Serial.begin(9600);
  portSetup();
  timerInterruptSetup();
  //CalibrationLoop();

  dumpClockSettings();
}








void loop() {
  /*
  int sensor = analogRead(SENSOR_PORT);
  if (sensor > port_threshold){
      int pitch = map(sensor, port_threshold, port_range_max, PITCH_RANGE_LOW, PITCH_RANGE_HIGH);
      tone(PIEZO_PORT, pitch, PITCH_DURATION);
  }
  delay(PLAY_LOOP_DELAY);
  */
  while (true){
    int i = 0;
    for (i=0; i<100; i++){
      delta_A = .2*i;
      delay(100);
    }
  }
}



void CalibrationLoop(){
  
  digitalWrite(LED_PORT, HIGH);

  while (millis() < SETUP_LOOP_DURATION_MILLIS){
    int sensorValue = analogRead(SENSOR_PORT);
    if (sensorValue < port_range_min){
      port_range_min = sensorValue;
    }
    if (sensorValue > port_range_max){
      port_range_max = sensorValue;
    }    
    
  }
  port_threshold = int(port_range_min + 0.1*(port_range_max - port_range_min));
  digitalWrite(LED_PORT, LOW);
  
}




