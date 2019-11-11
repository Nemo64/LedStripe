#include <Adafruit_NeoPixel.h>
#include "EffectRendering.h"

using namespace EffectRendering;

#define LED_PIN         6
#define LED_COUNT       100
#define SERIAL_TIMINGS  0

const auto EFFECT_DURATION = 8 * 1024;
const auto FADE_DURATION = 2 * 1024;
const auto FORWARDS = true;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(0xff / 2);
#if SERIAL_TIMINGS == 1
  Serial.begin(9600);
#endif
}

void loop() {
#if SERIAL_TIMINGS == 1
  auto startTime = micros();
#endif

  unsigned time = millis();
  unsigned effect = time / EFFECT_DURATION;
  unsigned progress = time % EFFECT_DURATION;

  renderWithTransition < EFFECT_DURATION, FADE_DURATION, FORWARDS, 0, 50 > (strip, effect, progress);
  renderWithTransition < EFFECT_DURATION, FADE_DURATION, FORWARDS, 50, 100 > (strip, effect - 1, progress);

#if SERIAL_TIMINGS == 1
  auto endTime = micros();
#endif

  strip.show();
  
#if SERIAL_TIMINGS == 1
  Serial.print(endTime - startTime);
  Serial.print("\t");
  Serial.println(micros() - endTime);
#endif
}
