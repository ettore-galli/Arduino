
#include "Arduino.h"

void setupTimer(int OutputCompareRegister1InitialValue)
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
    OCR1A = OutputCompareRegister1InitialValue;

    // enable global interrupts:
    sei();
}