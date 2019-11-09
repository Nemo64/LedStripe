#include <Adafruit_NeoPixel.h>
#include "Color.h"
#include "FadeEffect.h"
#include "RgbEffect.h"

#define LED_PIN         6
#define LED_COUNT       100

#define SWITCH_DURATION (1024 * 8)
#define FADE_DURATION   (1024 * 2)
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

const Color::color rgb[] = {0xff0000, 0x00ff00, 0x0000ff};
const Color::color wbbb[] = {0xffffff, 0x000000, 0x000000, 0x000000};
const Color::color fire[] = {0xff6600, 0xff8800, 0xff4400, 0xff2200};
const Color::color waves[] = {0xccffff, 0x44aaff, 0x0088ff, 0x0044ff, 0x0000ff, 0x0000ff, 0x0000ff};
      
void renderEffect(Adafruit_NeoPixel &strip, const int &effect, const uint16_t &first, const uint16_t &last) {
  switch (effect % 4) {
    case 0:
      RgbEffect::run<16>(strip, first, last, rgb);
      break;
    case 1:
      RgbEffect::run<8>(strip, first, last, wbbb);
      break;
    case 2:
      RgbEffect::run<8>(strip, first, last, fire);
      break;
    case 3:
      RgbEffect::run<8>(strip, first, last, waves);
      break;
  }
}
