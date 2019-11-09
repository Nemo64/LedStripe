#include <Adafruit_NeoPixel.h>
#include "FadeEffect.h"
#include "RgbEffect.h"

#define LED_PIN         6
#define LED_COUNT       100

#define EFFECT_COUNT    2
#define SWITCH_DURATION (1024 * 4)
#define FADE_DURATION   (1024)
#define STAY_DURATION   (SWITCH_DURATION - FADE_DURATION)
#define FADE_DIRECTION  1

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(0x0f);
}

void loop() {
  auto time = millis();
  auto effect = time / SWITCH_DURATION;
  auto fade = time % SWITCH_DURATION;
  if (fade > STAY_DURATION) {
    fade -= STAY_DURATION;
  } else {
    fade = 0;
  }

  auto fadePosition = strip.numPixels() * fade / FADE_DURATION;
  
  #if FADE_DIRECTION == 0
  renderEffect(strip, effect % EFFECT_COUNT, fadePosition, strip.numPixels());
  if (fadePosition > 0) {
    renderEffect(strip, (effect + 1) % EFFECT_COUNT, 0, fadePosition);
  }
  #endif

  #if FADE_DIRECTION == 1
  renderEffect(strip, effect % EFFECT_COUNT, 0, strip.numPixels() - fadePosition);
  if (fadePosition > 0) {
    renderEffect(strip, (effect + 1) % EFFECT_COUNT, strip.numPixels() - fadePosition, strip.numPixels());
  }
  #endif
  
  
  strip.show();
}

void renderEffect(Adafruit_NeoPixel &strip, const int &effect, const uint16_t &first, const uint16_t &last) {
  switch (effect) {
    case 0:
      RgbEffect::run(strip, first, last);
      break;
    case 1:
      FadeEffect::run(strip, first, last);
      break;
  }
}
