// outputs individually controllable sine waves on:
//
// pins: 9, 10, 11 and 3
//
// ********* to change pitch/volume of oscillators *******
//  oscillators[0].volume=127; // volume from 0...127
//  // pitch in hz
//  oscillators[0].phaseStep=hzToPhaseStep(440);
// 
//
//*********** TO CHANGE OSCILLATOR ROUTING: *******
//
// If you want to mix oscillators on one pin, you need to:
//   1) There are two interrupt functions below, ISR(TIMER1_OVF_vect) 
//      and ISR(TIMER2_OVF_vect). Each services two
//      output pins. You need to make sure that the oscillators
//      you are using to drive a pin are only updated in the
//      interrupt for that pin.
//      Note: you can't re-use oscillators across interrupts
//      
//   2) So, make sure all the oscillators you wish to use are
//      updated in the correct pin.
//
//   3) Then at the end, where the oscillator is written out to a
//      a pin, just add the value of the multiple oscillators 
//      together (look in the interrupt functions below, one
//      example is at the end).
//
//   4) Make sure your volumes of oscillators being mixed never 
//      total more than 127, or all hell will break loose in a bad 
//      and speaker breaking way.
//




char sine256[256]  __attribute__ ((aligned(256))) = {
0 , 3 , 6 , 9 , 12 , 15 , 18 , 21 , 24 , 27 , 30 , 33 , 36 , 39 , 42 , 45 , 
48 , 51 , 54 , 57 , 59 , 62 , 65 , 67 , 70 , 73 , 75 , 78 , 80 , 82 , 85 , 87 , 
89 , 91 , 94 , 96 , 98 , 100 , 102 , 103 , 105 , 107 , 108 , 110 , 112 , 113 , 114 , 116 , 
117 , 118 , 119 , 120 , 121 , 122 , 123 , 123 , 124 , 125 , 125 , 126 , 126 , 126 , 126 , 126 , 
127 , 126 , 126 , 126 , 126 , 126 , 125 , 125 , 124 , 123 , 123 , 122 , 121 , 120 , 119 , 118 , 
117 , 116 , 114 , 113 , 112 , 110 , 108 , 107 , 105 , 103 , 102 , 100 , 98 , 96 , 94 , 91 , 
89 , 87 , 85 , 82 , 80 , 78 , 75 , 73 , 70 , 67 , 65 , 62 , 59 , 57 , 54 , 51 , 
48 , 45 , 42 , 39 , 36 , 33 , 30 , 27 , 24 , 21 , 18 , 15 , 12 , 9 , 6 , 3 , 
0 , -3 , -6 , -9 , -12 , -15 , -18 , -21 , -24 , -27 , -30 , -33 , -36 , -39 , -42 , -45 , 
-48 , -51 , -54 , -57 , -59 , -62 , -65 , -67 , -70 , -73 , -75 , -78 , -80 , -82 , -85 , -87 , 
-89 , -91 , -94 , -96 , -98 , -100 , -102 , -103 , -105 , -107 , -108 , -110 , -112 , -113 , -114 , -116 , 
-117 , -118 , -119 , -120 , -121 , -122 , -123 , -123 , -124 , -125 , -125 , -126 , -126 , -126 , -126 , -126 , 
-127 , -126 , -126 , -126 , -126 , -126 , -125 , -125 , -124 , -123 , -123 , -122 , -121 , -120 , -119 , -118 , 
-117 , -116 , -114 , -113 , -112 , -110 , -108 , -107 , -105 , -103 , -102 , -100 , -98 , -96 , -94 , -91 , 
-89 , -87 , -85 , -82 , -80 , -78 , -75 , -73 , -70 , -67 , -65 , -62 , -59 , -57 , -54 , -51 , 
-48 , -45 , -42 , -39 , -36 , -33 , -30 , -27 , -24 , -21 , -18 , -15 , -12 , -9 , -6 , -3 
};

volatile char* curWave=sine256;

#include "avr/pgmspace.h"

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))



// set both timer to do phase correct PCM at
// a nominal frequency of 31372.549
void setupTimers() 
{
// Timer1 and 2 Clock Prescaler to : 1, set WGM22 to zero, and WGM12,13
// for phase correct PCM
  TCCR1B = (TCCR1B & 0b11100000) | 0b00001;
  TCCR2B = (TCCR2B & 0b11110000) | 0b0001;

// set it to clear compare match mode
// on both pins, and phase correct PCM (WGM21 and WGM20)
  TCCR1A = (TCCR1A &0b00001100)| 0b10100001;
  TCCR2A = (TCCR2A &0b00001100)| 0b10100001;

// set overflow interrupts enabled for timers 1 and 2
  TIMSK2 |= (1<<TOIE2);
  TIMSK1 |= (1<<TOIE1);
  
}

// information about the current state of a single oscillator
struct OscillatorState
{
  unsigned int phaseStep; // frequency of this oscillator
   char volume; // volume multiplier for this oscillator
   unsigned int phaseAccu; // accumulator that steps through
                           // this oscillator
                           // high byte = current sample index
};

// our 4 oscillators
struct OscillatorState oscillators[4];

//********* interrupt routines for each of the timer overflows ******

inline byte getByteLevel(int accumulator)
{
    char valOut=((unsigned int)(accumulator))>>7;
    valOut+=128;
    return (byte)valOut;
}


// interrupt for timer 1 overflow (pins 9 and 10)
// in this, we set the next value for the PWM for those pins
// ie. we set the sample value
ISR(TIMER1_OVF_vect) 
{
  // update sample position (ignore overflow, as 
  // we use the top byte to index into a 256 byte buffer
  // and the overflow means it loops through the buffer)

  //oscillator 0 update 
  oscillators[0].phaseAccu+=oscillators[0].phaseStep;
  int valOut0=curWave[oscillators[0].phaseAccu>>8]*oscillators[0].volume;

  //oscillator 1 update 
  oscillators[1].phaseAccu+=oscillators[1].phaseStep;
  int valOut1=curWave[oscillators[1].phaseAccu>>8]*oscillators[1].volume;

// write these two oscillators out
// to individual pins

// write to pin 9
  OCR1A=getByteLevel(valOut0);
  // write to pin 10
  OCR1B=getByteLevel(valOut1);
}
// interrupt for timer 2 overflow (pins 11 and 3)
// in this, we set the next value for the PWM for those pins
// ie. we set the sample value
ISR(TIMER2_OVF_vect) 
{
  // update sample position (ignore overflow, as 
  // we use the top byte to index into a 256 byte buffer
  // and the overflow means it loops through the buffer)
  
  // update the oscillators - make sure you update the oscillator
  // in the correct interrupt function, based on what pin you're 
  // outputting from
  
  // oscillator 2 update  
  oscillators[2].phaseAccu+=oscillators[2].phaseStep;
  int valOut2=curWave[oscillators[2].phaseAccu>>8]*oscillators[2].volume;
  
  // oscillator 3 update  
  oscillators[3].phaseAccu+=oscillators[3].phaseStep;
  int valOut3=curWave[oscillators[3].phaseAccu>>8]*oscillators[3].volume;
    
// write the oscillators to individual pins
  OCR2A=getByteLevel(valOut2); // write to pin 11
  OCR2B=getByteLevel(valOut3); // write pin 3
  
// if you want to mix multiple oscillators, put it in here instead
// e.g.  OCR2A=getByteLevel(valOut2+valOut3);

}

unsigned int hzToPhaseStep(float hz)
{
  float phaseStep= hz *2.0886902978652881446683197032183;
//  (pow(2,16) * frequency) / 31376.6
  return (unsigned int)phaseStep;
}

void setup()
{
    pinMode(3, OUTPUT);     // pin11= PWM  output / frequency output

    pinMode(9, OUTPUT);     // pin11= PWM  output / frequency output

  pinMode(10, OUTPUT);     // pin11= PWM  output / frequency output
  pinMode(11, OUTPUT);     // pin11= PWM  output / frequency output
  Serial.begin(9600);        // connect to the serial port
  oscillators[0].volume=127;
  oscillators[1].volume=127;
  oscillators[2].volume=127;
  oscillators[3].volume=127;
  
  oscillators[0].phaseStep=hzToPhaseStep(440);
  oscillators[1].phaseStep=hzToPhaseStep(220);
  oscillators[2].phaseStep=hzToPhaseStep(110);
  oscillators[3].phaseStep=hzToPhaseStep(880);
  setupTimers();
}

void loop()
{
  const int MAXVOL=127;
  for(int vol=0;vol<MAXVOL;vol++)
  {
    for(int c=0;c<4;c++)
    {
      oscillators[c].volume=vol;
    }
    Serial.println(vol);
    delay(20);
  }
  delay(500);
  for(int vol=MAXVOL;vol>=0;vol--)
  {
    for(int c=0;c<4;c++)
    {
      oscillators[c].volume=vol;
    }
    Serial.println(vol);
    delay(20);
  }
  delay(250);
}

