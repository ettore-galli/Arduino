
// SENSORE
const int SENSOR_PORT = A0;
const int LED_PORT = 3;
const int PIEZO_PORT = 8;
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
const int WAVETABLE_LEN = 16;
int sine[WAVETABLE_LEN] = {128, 176, 218, 246, 256, 246, 218, 176, 128, 79, 37, 9, 0, 9, 37, 79};
int* wave = sine;
float wave_frequency_A = 440.0;
float interrupt_1_frequency = 0.0;


int TCNT1init = 63000;
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
/**
 * Returns the wave value
 */
float wave_value(float deltaphase){
  return wave[3 % WAVETABLE_LEN];
}

void timerInterruptSetup(){
  noInterrupts();
  TCCR1A = 0x00; // No clock source (Timer/Counter stopped) 
  TCCR1B = 0x00; // No clock source (Timer/Counter stopped)
   
  TCNT1 = 0; // Counter

  // CS10=1, CS11=0, CS12=1 ==> Prescaler 1024
  int CS12SET=1;
  int CS11SET=0;
  int CS10SET=0;
  TCCR1B |= (CS12SET<<CS12) | (CS11SET<<CS11) | (CS10SET<<CS10); // 101=Prescaler 1024; 100=Prescaler 256

  // Interrupt Expected frequency
  interrupt_1_frequency = SYSTEM_CLOCK_FREQUENCY / 256.0;

  // WGM11, WGM10, WGM12 = 1 ==> Fast PWM, Top = OCRA
  TCCR1A |= (1<<WGM11) | (0<<WGM10); 
  TCCR1B |= (1<<WGM12); 
  
  // Set interrupt type to timer Overflow interrupt
  TIMSK1 |= (1<<TOIE1); 

  
  // Pin mapping
  // OCR1A = pin 9
  // OCR1B = pin 10
  interrupts();  
}

void portSetup(){
  
  // put your setup code here, to run once:
  pinMode(SENSOR_PORT, INPUT); // Photoresistor
  pinMode(LED_PORT, OUTPUT);   // Calibration indicator LED
  // pinMode(PIEZO_PORT, OUTPUT);   // PIEZO

  
}

 
 ISR(TIMER1_OVF_vect) { 
  Serial.println("TIMER 1");
  Serial.println(millis());
  if (led_status == HIGH){
    led_status = LOW;
  } else {
    led_status = HIGH;
  }
  digitalWrite(LED_PORT, led_status);
  TCNT1 = TCNT1init;

//wave_frequency_A
  
  OCR1A = wave_value(delta_phase(wave_frequency_A, interrupt_1_frequency, WAVETABLE_LEN));
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




