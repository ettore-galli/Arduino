

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

#include "measurements.h"
#include "waves.h"

#define BAUD_RATE 115200

/* Pin definitions*/

/* PWM OUTPUT
 *IMPLICITLY* defined setting COM1A1=1 / COM1A0=0
 Needs to be declared as output in order to work
*/
const int PWM_OUTPUT = 9;

/* Analog input pitch controller */
const int PITCH_CONTROL = A0;

/* Actual wave being used */
const uint8_t* wave = sine;

/* Wave tabe index and increment */
volatile float phase = 0;
volatile float phase_shift = 256;

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
