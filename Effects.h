#ifndef EFFECTS_H
#define EFFECTS_H

#include <Adafruit_NeoPixel.h>

namespace Effects {

typedef bool direction_t;
const direction_t FORWARD = true;
const direction_t REVERSE = false;

// an effect is basically a function pointer
typedef void (*effect_t)(Adafruit_NeoPixel &, uint16_t, uint16_t);

// this function runs an effect
// and it also adds a simple transition to it
// the progress variable goes from 0 - FADE_DURATION
const uint8_t FROM_START = 0;
const uint8_t FROM_END = 1;
const uint8_t TO_START = 2;
const uint8_t TO_END = 3;
template<unsigned DURATION, uint8_t MODE>
inline void run (Adafruit_NeoPixel &strip, effect_t effect, unsigned start, unsigned end, unsigned progress) {
  unsigned pixelProgress = (unsigned long)(end - start) * progress / DURATION;
  switch (MODE) {
    case FROM_START: start = start + pixelProgress; break;
    case FROM_END: end = end - pixelProgress; break;
    case TO_START: start = end - pixelProgress; break;
    case TO_END: end = start + pixelProgress; break;
  }

  if (start < end) {
    effect(strip, start, end);
  }
}


// remember if the startup sequence is active
// this will be set to false after the first fade duration is over
bool startup = true;

// this function is similar to #run, but will show no effect instead of effect1 as long as startup is true
template<unsigned DURATION, bool FORWARDS>
void runWithStartup (Adafruit_NeoPixel &strip, effect_t effect1, effect_t effect2, unsigned start, unsigned end, unsigned progress) {
  if (startup) {
    run < DURATION, FORWARDS ? TO_END : TO_START > (strip, effect2, start, end, progress);
  } else {
    run < DURATION, FORWARDS ? FROM_START : FROM_END > (strip, effect1, start, end, progress);
    run < DURATION, FORWARDS ? TO_END : TO_START > (strip, effect2, start, end, progress);
  }
}
}

#endif
