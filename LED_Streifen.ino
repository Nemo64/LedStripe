#include <Adafruit_NeoPixel.h>
#include "Effects.h"
#include "EffectAlternatingCycle.h"
#include "EffectCycle.h"
#include "EffectNoise.h"
#include "EffectWave.h"

#define SERIAL_TIMINGS  0

// led strip definition including sections on which the strips are split
const unsigned STRIP_1_0 = 0;
const unsigned STRIP_1_1 = STRIP_1_0 + 21;
const unsigned STRIP_1_2 = STRIP_1_1 + 14;
const unsigned STRIP_1_3 = STRIP_1_2 + 14;
Adafruit_NeoPixel strip1(STRIP_1_3, 6, NEO_BRG + NEO_KHZ800);

const unsigned STRIP_2_0 = 0;
const unsigned STRIP_2_1 = STRIP_2_0 + 15;
const unsigned STRIP_2_2 = STRIP_2_1 + 19;
const unsigned STRIP_2_3 = STRIP_2_2 + 19;
Adafruit_NeoPixel strip2(STRIP_2_3, 5, NEO_BRG + NEO_KHZ800);

// effect timing
// Make sure every effect is a power of 2 like 1024, 2048, 4096 etc.
// Otherwise divisions will be slow and overflows will have strange effects
const unsigned EFFECT_DURATION = 16 * 1024;
const unsigned FADE_DURATION = 2 * 1024;

// list of effects
// for best results: have a power of 2 effects, otherwise some effects will show up more often than others
// also: EFFECT_COUNT * EFFECT_DURATION must be <= 65536 since the timestamp is a 16 bit timestamp.
// with the default 16 seconds per effect, you should use 1, 2 or 4 effects or change the timestamp type.
const size_t EFFECT_COUNT = 4;
const Effects::effect_t effects[EFFECT_COUNT] = {
  EffectCycle::run<8, 8, true, 0xff0000, 0xff4400, 0x00ff00, 0x0000ff>, // RGB cycle
  EffectCycle::run<2, 4, true, 0x00ff99, 0x000000, 0x000000, 0x000000>, // street lights in the backseat
  EffectCycle::run<8, 8, true, 0xff0000, 0xff4400, 0x00ff00, 0x0000ff>, // RGB cycle
  EffectNoise::run<2, 256, 0x992000, 0xcc2500, 0xff3000, 0x661600>, // fire
};
const Effects::effect_t effectsR[EFFECT_COUNT] = {
  EffectCycle::run<8, 8, false, 0xff0000, 0xff4400, 0x00ff00, 0x0000ff>, // RGB cycle
  EffectCycle::run<2, 4, false, 0x00ff99, 0x000000, 0x000000, 0x000000>, // street lights in the backseat
  EffectCycle::run<8, 8, false, 0xff0000, 0xff4400, 0x00ff00, 0x0000ff>, // RGB cycle
  EffectNoise::run<2, 256, 0x992000, 0xcc2500, 0xff3000, 0x661600>, // fire
};
// these are unused effects that i experimented with
//EffectNoise::run<8, 64, 0x0000ff, 0x0066ff, 0x000000, 0x000000>, // police effect
//EffectAlternatingCycle::run<1, 2, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000>, // K.I.T.T
//EffectCycle::run<8, 8, true, 0xff4400, 0x00ff00, 0x00ffff, 0xff00ff>, // other color cycle
//EffectWave::run<32, 1024, 1024, true, 0x000010, 0x6699ff>, // wave experiment
//EffectCycle::run<2, 4, true, 0xff00ff, 0x000000, 0x000000, 0x000000>, // pink parade
//EffectCycle::run<4, 4, true, 0xff0000, 0xff0000, 0x00ff00, 0x00ff00>, // red/green

void setup() {
  strip1.begin();
  strip2.begin();
#if SERIAL_TIMINGS == 1
  Serial.begin(9600);
#endif
}

void loop() {

#if SERIAL_TIMINGS == 1
  auto startTime = micros();
#endif

  unsigned time = millis();
  unsigned progress = time % EFFECT_DURATION;
  unsigned fadeProgress = min(progress, FADE_DURATION);
  unsigned effect2 = time / EFFECT_DURATION % EFFECT_COUNT;
  unsigned effect1 = effect2 == 0 ? EFFECT_COUNT - 1 : effect2 - 1;

  if (Effects::startup && time > FADE_DURATION) {
    Effects::startup = false;
  }

  Effects::runWithStartup<FADE_DURATION, Effects::FORWARD>(strip1, effects[effect1], effects[effect2], STRIP_1_0, STRIP_1_1, fadeProgress);
  Effects::runWithStartup<FADE_DURATION, Effects::FORWARD>(strip1, effects[effect1], effects[effect2], STRIP_1_1, STRIP_1_2, fadeProgress);
  Effects::runWithStartup<FADE_DURATION, Effects::FORWARD>(strip1, effects[effect1], effects[effect2], STRIP_1_2, STRIP_1_3, fadeProgress);

  Effects::runWithStartup<FADE_DURATION, Effects::FORWARD>(strip2, effects[effect1], effects[effect2], STRIP_2_0, STRIP_2_1, fadeProgress);
  Effects::runWithStartup<FADE_DURATION, Effects::REVERSE>(strip2, effectsR[effect1], effectsR[effect2], STRIP_2_1, STRIP_2_2, fadeProgress);
  Effects::runWithStartup<FADE_DURATION, Effects::FORWARD>(strip2, effects[effect1], effects[effect2], STRIP_2_2, STRIP_2_3, fadeProgress);

#if SERIAL_TIMINGS == 1
  auto endTime = micros();
#endif

  strip1.show();
  strip2.show();

#if SERIAL_TIMINGS == 1
  auto processTime = endTime - startTime;
  auto transferTime = micros() - endTime;
  Serial.print(processTime);
  Serial.print("\t");
  Serial.println(transferTime);
#endif
}
