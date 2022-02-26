
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

volatile int changed = 0;

void setup() {
  
  portSetup();
  CalibrationLoop();
  interruptSetup();
  
}


void signal(){
  changed = 1;
}

void interruptSetup(){
  attachInterrupt(digitalPinToInterrupt(2), signal, CHANGE);
}

void portSetup(){
  
  // put your setup code here, to run once:
  pinMode(SENSOR_PORT, INPUT); // Photoresistor
  pinMode(LED_PORT, OUTPUT);   // Calibration indicator LED
  // pinMode(PIEZO_PORT, OUTPUT);   // PIEZO
  
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

void loop() {
  int sensor = analogRead(SENSOR_PORT);
  if (sensor > port_threshold){
      int pitch = map(sensor, port_threshold, port_range_max, PITCH_RANGE_LOW, PITCH_RANGE_HIGH);
      tone(PIEZO_PORT, pitch, PITCH_DURATION);
  }
  delay(PLAY_LOOP_DELAY);
}


