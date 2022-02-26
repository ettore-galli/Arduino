
// Stato dello switch
const int input0 = A0;
int switchState = 1;
int ledState = 0;
const int loopDelayBase = 200;
int loopDelay = loopDelayBase;

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(2, INPUT); // Pulsante
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  
}

void allLedsOff(){
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void switchLedOn(int led){
  int i;
  for (i=3; i<=5; i++){
    if (i == led){
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}

void doSwitchHighLoop(int intervallo){
  int i;
  for (i=3; i<=5; i++){
    switchLedOn(i);
    delay(intervallo);
  }
}

float readTempFromSensor(int sensor){
  return 100.0*((analogRead(sensor)/1024.0)*5 - 0.5);
}

void loop() {
  // put your main code here, to run repeatedly:

  switchState = digitalRead(2);
  if (switchState == HIGH){
    if (ledState == HIGH){
      ledState = LOW;
    } else {
      ledState = HIGH;
    }
    delay(loopDelay);
  }
  
  if (ledState == HIGH){
    doSwitchHighLoop(loopDelay); 
  } else {
    allLedsOff();
  }


  float temp = readTempFromSensor(input0);
  loopDelay = loopDelayBase/(5.0*(temp-20.0)/20.0);
  
  Serial.print("\n");
  Serial.print(loopDelay);
  
  delay(50);
  

  
}
