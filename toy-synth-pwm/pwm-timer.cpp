
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
    // CS12 CS11 CS10 Description
    // 0    0    0    No clock source (Timer/Counter stopped).
    // 0    0    1    clkI/O/1 (No prescaling)
    // 0    1    0    clkI/O/8 (From prescaler)
    // 0    1    1    clkI/O/64 (From prescaler)
    // 1    0    0    clkI/O/256 (From prescaler)
    // 1    0    1    clkI/O/1024 (From prescaler)
    // 1    1    0    External clock source on T1 pin. Clock on falling edge.
    // 1    1    1    External clock source on T1 pin. Clock on rising edge.
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