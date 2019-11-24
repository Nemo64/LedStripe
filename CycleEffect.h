#ifndef CYCLE_EFFECT_H
#define CYCLE_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace CycleEffect {
using namespace Color;

template <unsigned DENSITY = 16, unsigned SPEED_DIVIDER = 4, bool FORWARDS = true, color_t... COLOR_PARAMETERS>
void run (Adafruit_NeoPixel &strip, uint16_t first, uint16_t last) {
  color_t COLORS[] = {COLOR_PARAMETERS...};
  size_t COLOR_COUNT = sizeof(COLORS) / sizeof(color_t);
  unsigned PIXEL_TIME_SHIFT = MAX_PROGRESS / DENSITY;

  unsigned time = millis() / SPEED_DIVIDER;

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * PIXEL_TIME_SHIFT * (FORWARDS ? -1 : 1);
    auto progress = adjustedTime % MAX_PROGRESS;

    auto index2 = adjustedTime / MAX_PROGRESS % COLOR_COUNT;
    auto index1 = index2 == 0 ? COLOR_COUNT - 1 : index2 - 1;

    auto finalColor = transition(COLORS[index1], COLORS[index2], progress);
    strip.setPixelColor(i, finalColor);
  }
}
}

#endif
