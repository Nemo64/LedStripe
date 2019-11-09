#include <Adafruit_NeoPixel.h>
#include "FadeEffect.h"
#include "RgbEffect.h"

#define LED_PIN         6
#define LED_COUNT       100

#define SWITCH_DURATION (1024 * 4)
#define FADE_DURATION   (1024)
#define STAY_DURATION   (SWITCH_DURATION - FADE_DURATION)
#define FADE_DIRECTION  1

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(0x0f);
  Serial.begin(9600);
}

void loop() {
  auto startTime = micros();
  
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
  renderEffect(strip, effect, fadePosition, strip.numPixels());
  if (fadePosition > 0) {
    renderEffect(strip, effect + 1, 0, fadePosition);
  }
#endif

#if FADE_DIRECTION == 1
  renderEffect(strip, effect, 0, strip.numPixels() - fadePosition);
  if (fadePosition > 0) {
    renderEffect(strip, effect + 1, strip.numPixels() - fadePosition, strip.numPixels());
  }
#endif

  auto endTime = micros();
  Serial.print(endTime - startTime);
  strip.show();
  Serial.print("\t");
  Serial.println(micros() - endTime);
}

void renderEffect(Adafruit_NeoPixel &strip, const int &effect, const uint16_t &first, const uint16_t &last) {
  switch (effect % 2) {
    case 0:
      RgbEffect::run(strip, first, last);
      break;
    case 1:
      FadeEffect::run(strip, first, last);
      break;
  }
}
