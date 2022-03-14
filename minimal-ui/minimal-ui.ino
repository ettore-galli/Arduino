

/* MINIMAL UI */

#define BAUD_RATE 115200

const int BUTTON_INPUT = 2;
const int LED_OUTPUT = 7;

volatile boolean ledOn = false;

void setupSerial() { Serial.begin(BAUD_RATE); }

void setupGPIO()
{
    pinMode(LED_OUTPUT, OUTPUT);
    pinMode(BUTTON_INPUT, INPUT_PULLUP);
}

void setupButtonInterrupt() { attachInterrupt(digitalPinToInterrupt(BUTTON_INPUT), change, CHANGE); }

void change()
{
    ledOn = !digitalRead(BUTTON_INPUT);
}

void logLedEvent(bool ledStatus){
    if (ledStatus){
        Serial.println("*on");
    } else {
        Serial.println("*off");
    }
}

void RefreshOut()
{
    delay(10);
    //logLedEvent(ledOn);
    digitalWrite(LED_OUTPUT, ledOn);
}

void setup()
{
    setupSerial();
    setupGPIO();
    setupButtonInterrupt();
}

void loop() { RefreshOut(); }
