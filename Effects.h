#ifndef EFFECTS_H
#define EFFECTS_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

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
template<unsigned DURATION, uint8_t MODE, bool ANTI_ALIASING = false>
inline void run (Adafruit_NeoPixel &strip, effect_t effect, unsigned start, unsigned end, unsigned progress) {
  unsigned long totalProgress = (unsigned long)(end - start) * progress * Color::MAX_PROGRESS / DURATION;
  unsigned pixelProgress = totalProgress / Color::MAX_PROGRESS;

  unsigned fadeProgress = totalProgress % Color::MAX_PROGRESS;
  uint16_t fadePixel = 0;
  if (fadeProgress > 0) {
    pixelProgress += 1;
  }

  switch (MODE) {
    case FROM_START:
      start = start + pixelProgress;
      fadePixel = start - 1;
      break;
    case FROM_END:
      end = end - pixelProgress;
      fadePixel = end + 1;
      break;
    case TO_START:
      start = end - pixelProgress;
      fadePixel = start + 1;
      break;
    case TO_END:
      end = start + pixelProgress;
      fadePixel = end - 1;
      break;
  }

  if (start >= end) {
    return;
  }

  if (ANTI_ALIASING && fadeProgress > 0) {
    auto fadePixelBefore = strip.getPixelColor(fadePixel);
    effect(strip, start, end);
    auto betweenColor = Color::transition(fadePixelBefore, strip.getPixelColor(fadePixel), fadeProgress);
    strip.setPixelColor(fadePixel, betweenColor);
  } else {
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
    strip.fill(0x000000, start, end - start);
    run < DURATION, FORWARDS ? TO_END : TO_START, true > (strip, effect2, start, end, progress);
  } else {
    run < DURATION, FORWARDS ? FROM_START : FROM_END > (strip, effect1, start, end, progress);
    run < DURATION, FORWARDS ? TO_END : TO_START, true > (strip, effect2, start, end, progress);
  }
}
}

#endif
