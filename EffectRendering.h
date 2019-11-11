#ifndef EFFECT_RENDERING_H
#define EFFECT_RENDERING_H

#include "Color.h"
#include "RgbEffect.h"

namespace EffectRendering {

const Color::color rgb[] = {0xff0000, 0x00ff00, 0x0000ff};
const Color::color wbbb[] = {0xff6033, 0xff4411};
const Color::color fire[] = {0xff6600, 0xff8800, 0xff4400, 0xff2200};
const Color::color waves[] = {
  0xccffff, 0x44ccff, 0x11aaff, 0x0066ff,
  0x0022ff, 0x0011ff, 0x0000ff, 0x0000ff,
  0x0022ff, 0x0000ff, 0x0000ff, 0x0000ff,
  0x0022ff, 0x0000ff, 0x0000ff, 0x0000ff,
};

template <bool FORWARDS = true>
inline void render(Adafruit_NeoPixel &strip, const unsigned &effect, const uint16_t &first, const uint16_t &last) {
  switch (effect % 4) {
    case 0:
      RgbEffect::run<16, 4, FORWARDS>(strip, first, last, rgb);
      break;
    case 1:
      RgbEffect::run<2, 1, FORWARDS>(strip, first, last, wbbb);
      break;
    case 2:
      RgbEffect::run<8, 4, FORWARDS>(strip, first, last, fire);
      break;
    case 3:
      RgbEffect::run<2, 1, FORWARDS>(strip, first, last, waves);
      break;
  }
}

template <unsigned EFFECT_DURATION, unsigned FADE_DURATION, bool FORWARDS = true, uint16_t FIRST = 0, uint16_t LAST = 100>
inline void renderWithTransition (Adafruit_NeoPixel &strip, const unsigned &effect, const unsigned &progress) {
  const auto STAY_DURATION = EFFECT_DURATION - FADE_DURATION;

  unsigned fade = progress > STAY_DURATION ? progress - STAY_DURATION : 0;
  unsigned fadePosition = (unsigned long)(LAST - FIRST) * fade / FADE_DURATION;

  if (FORWARDS) {
    render<FORWARDS>(strip, effect, fadePosition + FIRST, LAST);
    if (fadePosition > 0) {
      render<FORWARDS>(strip, effect + 1, FIRST, fadePosition + FIRST);
    }
  } else {
    render<FORWARDS>(strip, effect, FIRST, LAST - fadePosition);
    if (fadePosition > 0) {
      render<FORWARDS>(strip, effect + 1, LAST - fadePosition, LAST);
    }
  }
}

}

#endif
