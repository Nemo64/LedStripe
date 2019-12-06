#ifndef EFFECT_ALTERNATING_CYCLE_H
#define EFFECT_ALTERNATING_CYCLE_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace EffectAlternatingCycle {
using namespace Color;

template <unsigned DENSITY, unsigned SPEED_DIVIDER, color_t... COLOR_PARAMETERS>
void run (Adafruit_NeoPixel &strip, uint16_t first, uint16_t last) {
  static const color_t COLORS[] = {COLOR_PARAMETERS...};
  static const size_t COLOR_COUNT = sizeof(COLORS) / sizeof(color_t);
  unsigned PIXEL_TIME_SHIFT = MAX_PROGRESS / DENSITY;

  unsigned DIRECTION_DURATION = COLOR_COUNT * MAX_PROGRESS;
  unsigned TOTAL_DURATION = DIRECTION_DURATION * 2;

  unsigned time = millis() / SPEED_DIVIDER % TOTAL_DURATION;
  if (time > DIRECTION_DURATION) {
    time = TOTAL_DURATION - time;
  }

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * PIXEL_TIME_SHIFT;
    auto progress = adjustedTime % MAX_PROGRESS;

    auto index2 = adjustedTime / MAX_PROGRESS % COLOR_COUNT;
    auto index1 = index2 == 0 ? COLOR_COUNT - 1 : index2 - 1;

    auto finalColor = transition(COLORS[index1], COLORS[index2], progress);
    strip.setPixelColor(i, finalColor);
  }
}
}

#endif
