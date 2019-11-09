#ifndef RGB_EFFECT_H
#define RGB_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace RgbEffect {
using namespace Color;

template <unsigned COLOR_WIDTH = 20, unsigned SPEED_DIVIDER = 4, size_t COLOR_COUNT>
void run (Adafruit_NeoPixel &strip, const uint16_t &first, const uint16_t &last, const color (&colors)[COLOR_COUNT]) {
  unsigned PIXEL_TIME_SHIFT = MAX_PROGRESS / COLOR_WIDTH;
  unsigned time = millis() / SPEED_DIVIDER;

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * PIXEL_TIME_SHIFT;
    auto progress = adjustedTime % MAX_PROGRESS;
    auto index = adjustedTime / MAX_PROGRESS % COLOR_COUNT;
    auto color1 = colors[index];
    auto color2 = colors[index + 1 == COLOR_COUNT ? 0 : index + 1];
    strip.setPixelColor(i, transition(color1, color2, progress));
  }
}
}

#endif
