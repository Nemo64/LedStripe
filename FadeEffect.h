#ifndef FADE_EFFECT_H
#define FADE_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace FadeEffect {
  using namespace Color;

  const color colors[] = {0xff0000, 0x00ff00, 0x0000ff};
  const uint8_t COLOR_COUNT = 3;
  
  void run (Adafruit_NeoPixel &strip, const uint16_t &first, const uint16_t last) {
    auto time = millis() / 4;
    auto progress = time % MAX_PROGRESS;
    auto index = time / MAX_PROGRESS;
    auto color1 = colors[(index) % COLOR_COUNT];
    auto color2 = colors[(index + 1) % COLOR_COUNT];
    auto finalColor = transition(color1, color2, progress);
    strip.fill(finalColor, first, last - first);
  }
}

#endif
