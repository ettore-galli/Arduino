
// Stato dello switch
const int input0 = A0;
const float pi = 3.141592;

int switchState = 1;
int ledState = 0;
const int loopDelayBase = 200;
int loopDelay = loopDelayBase;
int ledbright = 0;

int ledA=3;
int ledB=5;
int ledC=6;

int cur_step_A = 0;
int cur_step_B = 0;
int cur_step_C = 0;

int STEPS = 256;

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(2, INPUT); // Pulsante
  
  pinMode(ledA, OUTPUT); 
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);  

  
}



void loop() {
  int stepsA = 1.0 * STEPS;
  int stepsB = 1.5 * STEPS;
  int stepsC = 3.0 * STEPS;
  
  cur_step_A = (cur_step_A + 1) % stepsA;
  cur_step_B = (cur_step_B + 1) % stepsB;
  cur_step_C = (cur_step_C + 1) % stepsC;
  
  float angA = pi * cur_step_A / stepsA;
  float angB = pi * cur_step_B / stepsB;
  float angC = pi * cur_step_C / stepsC;
  
  int outA = 255 * (sin(angA));  
  int outB = 255 * (sin(angB)); 
  int outC = 255 * (sin(angC)); 

  analogWrite(ledA, outA);
  analogWrite(ledB, outB);
  analogWrite(ledC, outC);

  /**
  Serial.print("\n");
  Serial.print(sin(angA));
  Serial.print("-");
  Serial.print(sin(angB));
  Serial.print("-");
  Serial.print(sin(angC));
  **/
  
  int rit = 1024/STEPS;
  
  delay(rit);
  
}


