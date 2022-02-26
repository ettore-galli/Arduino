/**
   MONITOR INPUT ANALOGICI
*/

// Serial port settings
const int BAUD_RATE_9600 = 9600;
const int BAUD_RATE_14400 = 14400;
const int BAUD_RATE_19200 = 19200;
const int BAUD_RATE_28800 = 28800;
const int BAUD_RATE_38400 = 38400;
const int BAUD_RATE_56K = 57600;

const long SERIAL_BAUD_RATE = 1000000;
const int ANALOG_PORTS[1] = {A0};

const int LED_PORT = 3;

const int NUMBER_OF_SAMPLES = 16;
const int SAMPLE_SIZE = 2;
const char ENCODING_ALPHABET[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', '2', '3', '4', '5', '6', '7'
};


// Loop delay
const int TX_LOOP_DELAY_MICROSECONDS = 122; // ~ 8192 Khz freq. campionamento
int TX_LOOP_DELAY_MICROSECONDS_MULTIPLIER = 8;

// Sample counter and buffer
int samples = 0;
const int SAMPLE_BUFFER_SIZE = NUMBER_OF_SAMPLES * SAMPLE_SIZE + 1;
char sampleBuffer[SAMPLE_BUFFER_SIZE];

/**
   Main setup procedure
*/
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  portSetup();
}


int numberOfPorts() {
  return sizeof(ANALOG_PORTS) / sizeof(ANALOG_PORTS[0]);
}

/*
   Input/Outpurt ports setup
*/
void portSetup() {

  // Configurazione porte analogiche
  int i;
  for (i = 0; i < numberOfPorts(); i++) {
    pinMode(ANALOG_PORTS[i], INPUT);
  }

  // Pin digitali
  pinMode(LED_PORT, OUTPUT);

  pinMode(10, INPUT);
  pinMode(11, INPUT);

}

/**

*/
void resetBuffer() {
  samples = 0;
}

/**
   Encode integer samples < 1024
   Encode integer samples in two "ascii digits"
   similar to
*/
char *encodeSample(int s) {
  static char es[2];
  es[0] = ENCODING_ALPHABET[s / 32];
  es[1] = ENCODING_ALPHABET[s % 32];
  return es;
}

/**
   Write data in the proper position
*/
void appendToBuffer(char *s) {
  samples++;
  sampleBuffer[2 * (samples - 1)] = s[0];
  sampleBuffer[2 * (samples - 1) + 1] = s[1];
}

/**
   Send data if buffer is full, then reset
*/
void sendBufferIfFull() {
  if (samples == NUMBER_OF_SAMPLES) {
    sampleBuffer[SAMPLE_BUFFER_SIZE - 1] = '\n';
    Serial.write(sampleBuffer, SAMPLE_BUFFER_SIZE);
    resetBuffer();
  }
}

/**
   Main line
*/
void loop() {

  int data[numberOfPorts()];
  int i, ms1, ms2, msdelta;
  int s;

  ms1 = micros();

  s = analogRead(ANALOG_PORTS[0]);

  char * bs = encodeSample(s);
  appendToBuffer(bs);

  sendBufferIfFull();

  ms2 = micros();
  msdelta = ms2 - ms1;
  int loopDelay = TX_LOOP_DELAY_MICROSECONDS * TX_LOOP_DELAY_MICROSECONDS_MULTIPLIER;
  if ((msdelta > 0) && (msdelta < loopDelay)) {
    delayMicroseconds(loopDelay - msdelta);
  }

}







