

/*
    ----------------------------
    HARDWARE TIMER PWM TOY SYNTH
    ----------------------------


    ----- VARIOUS DOCS -----
    Headers
    https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/Arduino.h

    Stackexchange "Where is documentation?"
    https://arduino.stackexchange.com/questions/12382/where-is-documentation-on-arduinos-internal-interrupts

    Timer tutorial
    https://github.com/ElektorLabs/Arduino/blob/master/source2/avr/cores/arduino/avr/iom328p.h

    Interrupts
    http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html

    Datasheet
    https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

    Timer Interrupt Setup Example
    https://forum.arduino.cc/t/arduin-o-scope-a-tiny-oscilloscope-project/8138


    ----- GENERATE THE WAVETABLE -----
    ```python
    import math

    SAMPLES=256

    sine_wavetable = [int(127 * (1 + math.sin(2 * x * math.pi / SAMPLES))) for x in range(SAMPLES)]
    ```
*/

#define BAUD_RATE 115200

/* Pin definitions*/

/* PWM OUTPUT
 *IMPLICITLY* defined setting COM1A1=1 / COM1A0=0
 Needs to be declared as output in order to work
*/
const int PWM_OUTPUT = 9;

/* Analog input pitch controller */
const int PITCH_CONTROL = A0;

/* Wave table */
const int SAMPLES = 256;
const uint8_t sine[SAMPLES] = { 127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175,
    178, 181, 184, 186, 189, 192, 194, 197, 200, 202, 205, 207, 209, 212, 214, 216, 218, 221, 223, 225, 227, 229, 230,
    232, 234, 235, 237, 239, 240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 253,
    253, 254, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 247, 246, 245, 244, 243, 241, 240, 239, 237,
    235, 234, 232, 230, 229, 227, 225, 223, 221, 218, 216, 214, 212, 209, 207, 205, 202, 200, 197, 194, 192, 189, 186,
    184, 181, 178, 175, 172, 169, 166, 163, 160, 157, 154, 151, 148, 145, 142, 139, 136, 133, 130, 127, 123, 120, 117,
    114, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 72, 69, 67, 64, 61, 59, 56, 53, 51, 48, 46, 44, 41, 39,
    37, 35, 32, 30, 28, 26, 24, 23, 21, 19, 18, 16, 14, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 16, 18, 19, 21, 23, 24, 26, 28, 30, 32, 35, 37, 39,
    41, 44, 46, 48, 51, 53, 56, 59, 61, 64, 67, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96, 99, 102, 105, 108, 111, 114,
    117, 120, 123 };

/* Actual wave being used */
const uint8_t* wave = sine;

/* Wave tabe index and increment */
volatile float phase = 0;
volatile float phase_shift = 256;

const int NREADINGS = 10;
volatile float readings[NREADINGS];
volatile int readingsIdx = 0;

void setNewReading(int reading)
{
    readings[readingsIdx] = reading;
    readingsIdx = (readingsIdx + 1) % NREADINGS;
}

float getReadingsAverage()
{
    float sum = 0.0;
    for (int i = 0; i < NREADINGS; i++) {
        sum += readings[i];
    }
    return sum / NREADINGS;
}

void setupTimer()
{

    // initialize Timer1
    cli(); // disable global interrupts

    // Reset all registers of concern
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TIMSK1 = 0x00;

    // WGM 10/11/12 = 1 0 0 = PWM, Phase Correct, 8-bit
    TCCR1A |= (1 << WGM10);
    TCCR1A |= (0 << WGM11);
    TCCR1B |= (0 << WGM12);

    // "Clear PWM_OUTPUT/OC1B on Compare Match, set OC1A/OC1B at BOTTOM (non-inverting mode)"
    TCCR1A |= (1 << COM1A1) | (0 << COM1A0);

    // Set prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (0 << CS11);
    TCCR1B |= (0 << CS12);

    // Enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // Set Output compare phase (max is 32767)
    OCR1A = 256;

    // enable global interrupts:
    sei();
}

void setupSerial() { Serial.begin(BAUD_RATE); }

void setupGPIO()
{
    pinMode(PWM_OUTPUT, OUTPUT);
    pinMode(PITCH_CONTROL, INPUT);
}

/**
   Timer 1 Overflow Interrupt Service Routine
   Triggered when TOIE1 = 1
*/
ISR(TIMER1_OVF_vect)
{
    OCR1A = wave[(uint8_t)phase];
    phase = phase + phase_shift - (phase > SAMPLES ? SAMPLES : 0);
}

void ToneSweepLoopDemo()
{
    for (int shift = 0; shift < 100; shift++) {
        phase_shift = 0.3 * shift;
        Serial.println(phase_shift);
        delay(50);
    }
}

void InputPitch()
{
    setNewReading(analogRead(PITCH_CONTROL));

    phase_shift = 50.0 * getReadingsAverage() / 1024;
    // Serial.println(phase_shift);

    delay(10);
}

void setup()
{
    setupSerial();
    setupGPIO();
    setupTimer();

    ToneSweepLoopDemo();
}

void loop() { InputPitch(); }
