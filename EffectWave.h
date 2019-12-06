#ifndef EFFECT_WAVE_H
#define EFFECT_WAVE_H

#include <Adafruit_NeoPixel.h>
#include "Color.h"

namespace EffectWave {
using namespace Color;

template <unsigned DENSITY, unsigned ATTACK, unsigned DECAY, bool FORWARDS, color_t GROUND_COLOR, color_t WAVE_COLOR>
void run (Adafruit_NeoPixel &strip, uint16_t first, uint16_t last) {
  unsigned time = millis();
  unsigned progress = time % (ATTACK + DECAY);

  for (auto i = first; i < last; i += DENSITY) {
    auto waveStart = i / DENSITY * DENSITY; // round to a fixed position
    auto waveEnd = waveStart + DENSITY;

    uint16_t waveProgress = 0;
    if (progress <= ATTACK) {
      waveProgress = (unsigned long)progress * DENSITY / ATTACK;
    } else {
      waveProgress = DENSITY - (unsigned long)(progress - ATTACK) * DENSITY / DECAY;
    }

    // make the wave movement exponential
    auto inversedWaveProgress = DENSITY - waveProgress;
    waveProgress = DENSITY - inversedWaveProgress * inversedWaveProgress / DENSITY;

    auto waveMiddle = FORWARDS ? waveStart + waveProgress : waveEnd - waveProgress;

    // print the fade in side (from the beginning of the strip)
    for (auto j = i; j < min(waveMiddle, last); ++j) {
      auto colorProgress = (j - waveStart) * MAX_PROGRESS / (FORWARDS ? waveProgress : DENSITY - waveProgress);
      auto color = transition(GROUND_COLOR, WAVE_COLOR, colorProgress * colorProgress / MAX_PROGRESS * waveProgress / DENSITY);
      strip.setPixelColor(j, color);
    }

    // print the fade out side (from the beginning of the strip)
    for (auto j = max(i, waveMiddle); j < min(waveEnd, last); ++j) {
      auto colorProgress = MAX_PROGRESS - (j - waveMiddle) * MAX_PROGRESS / (FORWARDS ? DENSITY - waveProgress : waveProgress);
      auto color = transition(GROUND_COLOR, WAVE_COLOR, colorProgress * colorProgress / MAX_PROGRESS * waveProgress / DENSITY);
      strip.setPixelColor(j, color);
    }
  }
}
}

#endif
