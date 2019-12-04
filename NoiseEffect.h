#ifndef NOISE_EFFECT_H
#define NOISE_EFFECT_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace NoiseEffect {
using namespace Color;

// this table is stolen from m_random.c of DOOM
const uint8_t rndtable[256] = {
  0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66 ,
  74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36 ,
  95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188 ,
  52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224 ,
  149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242 ,
  145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0 ,
  175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235 ,
  25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113 ,
  94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75 ,
  136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196 ,
  135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113 ,
  80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241 ,
  24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224 ,
  145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95 ,
  28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226 ,
  71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36 ,
  17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106 ,
  197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136 ,
  120, 163, 236, 249
};
const unsigned RANDOM_NUMBERS = sizeof(rndtable) / sizeof(rndtable[0]);

template <color_t... COLOR_PARAMETERS>
inline color_t getColor(unsigned roll, unsigned position, unsigned progress) {
  color_t COLORS[] = {COLOR_PARAMETERS...};
  size_t COLOR_COUNT = sizeof(COLORS) / sizeof(color_t);
  
  auto adjustedRoll = roll + position * 32;
  auto index1 = adjustedRoll % RANDOM_NUMBERS;
  auto index2 = index1 + 1 == RANDOM_NUMBERS ? 0 : index1 + 1;

  auto color1 = COLORS[rndtable[index1] % COLOR_COUNT];
  auto color2 = COLORS[rndtable[index2] % COLOR_COUNT];
  return transition(color1, color2, progress);
}

template <unsigned DENSITY, unsigned ROLL_TIME, color_t... COLOR_PARAMETERS>
void run (Adafruit_NeoPixel &strip, uint16_t first, uint16_t last) {

  unsigned time = millis();
  auto progress = (unsigned long)(time % ROLL_TIME) * MAX_PROGRESS / ROLL_TIME;
  auto roll = time / ROLL_TIME;

  // the strip will be split into segments of color which will be faded between
  // to avoid repeated calculation of the same color, I calculate them all in the beginning
  // since there is a maximum of 300 leds (i think), the memory footprint of this could be 156 bytes which is fine imo
  auto firstSegment = first / DENSITY;
  auto lastSegment = (last + DENSITY - 1) / DENSITY + 1; // round up and since there is always a fade, I need to calculate 1 segment more
  color_t colors[lastSegment - firstSegment]; // that this works is awesome: dynamic array sinze in stack
  for (auto segment = firstSegment; segment < lastSegment; ++segment) {
    colors[segment - firstSegment] = getColor<COLOR_PARAMETERS...>(roll, segment, progress);
  }

  for (auto i = first; i < last; ++i) {
    auto colorIndex = i / DENSITY - firstSegment;
    auto color1 = colors[colorIndex];
    auto color2 = colors[colorIndex + 1];
    auto fade = i % DENSITY * MAX_PROGRESS / DENSITY;
    auto finalColor = transition(color1, color2, fade);
    strip.setPixelColor(i, finalColor);
  }
}
}

#endif
