/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include <Arduino.h>
#include "Arduboy2Beep.h"

#if defined(_ADAFRUIT_ARCADA_)
#include <Audio.h>
extern Adafruit_Arcada arcada;

AudioSynthWaveformSine   sine1, sine2;
AudioMixer4              mixer1;
AudioOutputAnalogStereo  audioOut;
AudioConnection          patchCord1(sine2, 0, mixer1, 1);
AudioConnection          patchCord2(sine1, 0, mixer1, 0);
AudioConnection          patchCord3(mixer1, 0, audioOut, 0);
AudioConnection          patchCord4(mixer1, 0, audioOut, 1);

#define ARCADA_MAX_VOLUME 0.5

uint8_t BeepPin1::duration = 0;
uint8_t BeepPin2::duration = 0;

void BeepPin1::begin()
{
  AudioMemory(2);
  arcada.enableSpeaker(true);
}

void BeepPin2::begin()
{
  AudioMemory(2);
  arcada.enableSpeaker(true);
}

void BeepPin1::tone(float freq)
{
  BeepPin1::tone(freq, 0);
}

void BeepPin2::tone(float freq)
{
  BeepPin2::tone(freq, 0);
}

void BeepPin1::tone(float freq, uint8_t dur)
{
  //Serial.printf("Tone1 %f Hz, %d dur\n", freq, dur);
  sine1.amplitude(ARCADA_MAX_VOLUME);
  sine1.frequency(freq);
  duration = dur;
}

void BeepPin2::tone(float freq, uint8_t dur)
{
  //Serial.printf("Tone2 %f Hz, %d dur\n", freq, dur);
  sine2.amplitude(ARCADA_MAX_VOLUME);
  sine2.frequency(freq);
  duration = dur;
}

void BeepPin1::timer()
{
  if (duration && (--duration == 0)) {
    sine1.amplitude(0);
  }
}

void BeepPin2::timer()
{
  if (duration && (--duration == 0)) {
    sine2.amplitude(0);
  }
}

void BeepPin1::noTone()
{
  sine1.amplitude(0);
}

void BeepPin2::noTone()
{
  sine2.amplitude(0);
}

#elif !defined(AB_DEVKIT)

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
  TCCR3A = 0;
  TCCR3B = (bit(WGM32) | bit(CS31)); // CTC mode. Divide by 8 clock prescale
}

void BeepPin1::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
  duration = dur;
  TCCR3A = bit(COM3A0); // set toggle on compare mode (which connects the pin)
  OCR3A = count; // load the count (16 bits), which determines the frequency
}

void BeepPin1::timer()
{
  if (duration && (--duration == 0)) {
    TCCR3A = 0; // set normal mode (which disconnects the pin)
  }
}

void BeepPin1::noTone()
{
  duration = 0;
  TCCR3A = 0; // set normal mode (which disconnects the pin)
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13
uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
  TCCR4A = 0; // normal mode. Disable PWM
  TCCR4B = bit(CS43); // divide by 128 clock prescale
  TCCR4D = 0; // normal mode
  TC4H = 0;  // toggle pin at count = 0
  OCR4A = 0; //  "
}

void BeepPin2::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
  duration = dur;
  TCCR4A = bit(COM4A0); // set toggle on compare mode (which connects the pin)
  TC4H = highByte(count); // load the count (10 bits),
  OCR4C = lowByte(count); //  which determines the frequency
}

void BeepPin2::timer()
{
  if (duration && (--duration == 0)) {
    TCCR4A = 0; // set normal mode (which disconnects the pin)
  }
}

void BeepPin2::noTone()
{
  duration = 0;
  TCCR4A = 0; // set normal mode (which disconnects the pin)
}


#else /* AB_DEVKIT */

// *** The pins used for the speaker on the DevKit cannot be directly
// controlled by a timer/counter. The following "dummy" functions will
// compile and operate properly but no sound will be produced

uint8_t BeepPin1::duration = 0;

void BeepPin1::begin()
{
}

void BeepPin1::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
  (void) count; // parameter not used

  duration = dur;
}

void BeepPin1::timer()
{
  if (duration) {
    --duration;
  }
}

void BeepPin1::noTone()
{
  duration = 0;
}


uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
}

void BeepPin2::tone(uint16_t count)
{
  tone(count, 0);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
  (void) count; // parameter not used

  duration = dur;
}

void BeepPin2::timer()
{
  if (duration) {
    --duration;
  }
}

void BeepPin2::noTone()
{
  duration = 0;
}

#endif
