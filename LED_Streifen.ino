#include <Adafruit_NeoPixel.h>
#include "CycleEffect.h"
#include "NoiseEffect.h"

#define LED_PIN         6
#define LED_COUNT       100
#define SERIAL_TIMINGS  0

const auto EFFECT_DURATION = 16 * 1024;
const auto FADE_DURATION = 2 * 1024;
const auto STAY_DURATION = EFFECT_DURATION - FADE_DURATION;
const auto FORWARDS = true;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

typedef void (*effect_t)(Adafruit_NeoPixel &, uint16_t, uint16_t);
const effect_t effects[] = {
  CycleEffect::run<16, 4, FORWARDS, 0xff0000, 0x00ff00, 0x0000ff>,
  NoiseEffect::run<8, 64, 0x992000, 0xcc2500, 0xff3000, 0x661600>,
};
const size_t EFFECT_COUNT = sizeof(effects) / sizeof(effects[0]);

void setup() {
  strip.begin();
  strip.setBrightness(0xff);
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
  unsigned fadeProgress = progress > STAY_DURATION ? progress - STAY_DURATION : 0;
  unsigned split = (unsigned long)(fadeProgress) * LED_COUNT / FADE_DURATION;

  effects[effect % EFFECT_COUNT](strip, split, LED_COUNT);
  if (fadeProgress > 0) {
    effects[(effect + 1) % EFFECT_COUNT](strip, 0, split);
  }

#if SERIAL_TIMINGS == 1
  auto endTime = micros();
#endif

  strip.show();

#if SERIAL_TIMINGS == 1
  auto finalTime = micros();
  Serial.print(endTime - startTime);
  Serial.print("\t");
  Serial.print(finalTime - endTime);
  Serial.print("\t");
  Serial.println(finalTime - startTime);
#endif
}
