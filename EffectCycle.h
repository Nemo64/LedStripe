#ifndef EFFECT_CYCLE_H
#define EFFECT_CYCLE_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace EffectCycle {
using namespace Color;

template <unsigned DENSITY, unsigned SPEED_DIVIDER, bool FORWARDS, color_t... COLOR_PARAMETERS>
void run (Adafruit_NeoPixel &strip, uint16_t first, uint16_t last) {
  static const color_t COLORS[] = {COLOR_PARAMETERS...};
  static const size_t COLOR_COUNT = sizeof(COLORS) / sizeof(color_t);
  static_assert(COLOR_COUNT >= 1, "There must be at least 1 color defined");
  unsigned PIXEL_TIME_SHIFT = MAX_PROGRESS / DENSITY;

  unsigned time = millis() / SPEED_DIVIDER;

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * (FORWARDS ? -PIXEL_TIME_SHIFT : PIXEL_TIME_SHIFT);
    auto progress = adjustedTime % MAX_PROGRESS;

    auto index2 = adjustedTime / MAX_PROGRESS % COLOR_COUNT;
    auto index1 = index2 == 0 ? COLOR_COUNT - 1 : index2 - 1;

    auto finalColor = transition(COLORS[index1], COLORS[index2], progress);
    strip.setPixelColor(i, finalColor);
  }
}
}

#endif
