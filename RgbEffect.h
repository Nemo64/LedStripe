#ifndef RGB_EFFECT_H
#define RGB_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace RgbEffect {
using namespace Color;

const color colors[] = {0xff0000, 0x00ff00, 0x0000ff};
const uint8_t COLOR_WIDTH = 20;

const uint8_t COLOR_COUNT = sizeof(colors) / sizeof(color);
const uint8_t PIXEL_TIME_SHIFT = MAX_PROGRESS / COLOR_WIDTH;

void run (Adafruit_NeoPixel &strip, const uint16_t &first, const uint16_t last) {
  uint16_t time = millis() / 4;

  for (auto i = first; i < last; ++i) {
    auto adjustedTime = time + i * PIXEL_TIME_SHIFT;
    auto progress = adjustedTime % MAX_PROGRESS;
    auto index = adjustedTime / MAX_PROGRESS;
    auto color1 = colors[(index) % COLOR_COUNT];
    auto color2 = colors[(index + 1) % COLOR_COUNT];
    strip.setPixelColor(i, transition(color1, color2, progress));
  }
}
}

#endif
