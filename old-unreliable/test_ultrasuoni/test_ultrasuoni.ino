const int ULTRASONIC_TRIG_PIN = 11; 
const int ULTRASONIC_ECHO_PIN = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT); 
  pinMode(ULTRASONIC_ECHO_PIN, INPUT); 
  Serial.begin(9600);
}

void loop() {
  /* ULTRASONIC DISTANCE */
  float duration, distance;
  duration = 0;
  int N=1;
  for (int i=0; i<N; i++){ 
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW); 
    delayMicroseconds(2); 
    digitalWrite(ULTRASONIC_TRIG_PIN, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  
    duration += pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

    delay(7);
  }
  duration = duration /N;
  distance = (duration*.0343)/2; 
  

  Serial.print(duration);
  Serial.print("\n");
 
  
}
