#ifndef RGB_EFFECT_H
#define RGB_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace RgbEffect {
using namespace Color;

template <unsigned COLOR_WIDTH = 20, unsigned SPEED_DIVIDER = 4, bool FORWARDS = true, size_t COLOR_COUNT>
void run (Adafruit_NeoPixel &strip, const uint16_t &first, const uint16_t &last, const color (&colors)[COLOR_COUNT]) {
  unsigned PIXEL_TIME_SHIFT = MAX_PROGRESS / COLOR_WIDTH;
  unsigned time = millis() / SPEED_DIVIDER;

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * PIXEL_TIME_SHIFT * (FORWARDS ? -1 : 1);
    auto progress = adjustedTime % MAX_PROGRESS;

    auto index1 = adjustedTime / MAX_PROGRESS % COLOR_COUNT;
    auto index2 = index1 + 1;
    if (index2 == COLOR_COUNT) {
      index2 = 0;
    }

    auto finalColor = transition(colors[index1], colors[index2], progress);
    strip.setPixelColor(i, finalColor);
  }
}
}

#endif
