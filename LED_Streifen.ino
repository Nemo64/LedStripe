#include <Adafruit_NeoPixel.h>
#include "AlternatingCycleEffect.h"
#include "CycleEffect.h"
#include "NoiseEffect.h"
#include "WaveEffect.h"

#define LED_PIN         6
#define LED_COUNT       100
#define SERIAL_TIMINGS  0

// effect timing
// Make sure every effect is a power of 2 like 1024, 2048, 4096 etc.
// Otherwise divisions will be slow and overflows will have strange effects
const unsigned EFFECT_DURATION = 16 * 1024;
const unsigned FADE_DURATION = 4 * 1024;

// led strip definition
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_BRG + NEO_KHZ800);

// list of effects
// an effect is basically a function pointer
// for best results: have a power of 2 effects, otherwise some effects will show up more often than others
// also: EFFECT_COUNT * EFFECT_DURATION must be <= 65536 since the timestamp is a 16 bit timestamp.
// with the default 16 seconds per effect, you should use 1, 2 or 4 effects or change the timestamp type.
typedef void (*effect_t)(Adafruit_NeoPixel &, uint16_t, uint16_t);
const effect_t effects[] = {
  //NoiseEffect::run<8, 64, 0x0000ff, 0x0066ff, 0x000000, 0x000000>, // police effect
  //AlternatingCycleEffect::run<1, 2, 0xff0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000>, // K.I.T.T
  //CycleEffect::run<8, 8, true, 0xff4400, 0x00ff00, 0x00ffff, 0xff00ff>, // other color cycle
  //WaveEffect::run<32, 1024, 1024, true, 0x000010, 0x6699ff>, // wave experiment
  NoiseEffect::run<8, 64, 0x992000, 0xcc2500, 0xff3000, 0x661600>, // fire
  CycleEffect::run<8, 4, true, 0xff0000, 0xff4400, 0x00ff00, 0x0000ff>, // RGB cycle
  //CycleEffect::run<2, 4, true, 0xff00ff, 0x000000, 0x000000, 0x000000>, // pink parade
  //CycleEffect::run<4, 8, true, 0xff0000, 0xff0000, 0x00ff00, 0x00ff00>, // red/green
};
const size_t EFFECT_COUNT = sizeof(effects) / sizeof(effects[0]);

// remember if the startup sequence is active
// this will be set to false after the first fade duration is over
bool startup = true;

void setup() {
  strip.begin();
#if SERIAL_TIMINGS == 1
  Serial.begin(9600);
#endif
}

// this function runs an effect
// and it also adds a simple transition to it
// the progress variable goes from 0 - FADE_DURATION
const uint8_t FROM_START = 0;
const uint8_t FROM_END = 1;
const uint8_t TO_START = 2;
const uint8_t TO_END = 3;
template<unsigned DURATION = FADE_DURATION>
inline void runEffect (Adafruit_NeoPixel &strip, effect_t effect, unsigned start, unsigned end, unsigned progress, uint8_t mode) {
  unsigned pixelProgress = (unsigned long)(end - start) * progress / DURATION;
  switch (mode) {
    case FROM_START: start = start + pixelProgress; break;
    case FROM_END: end = end - pixelProgress; break;
    case TO_START: start = end - pixelProgress; break;
    case TO_END: end = start + pixelProgress; break;
  }

  if (start < end) {
    effect(strip, start, end);
  }
}

#if SERIAL_TIMINGS == 1
unsigned long lastLoopTime = 0;
#endif

void loop() {

#if SERIAL_TIMINGS == 1
  auto startTime = micros();
#endif

  unsigned time = millis();
  unsigned progress = time % EFFECT_DURATION;
  unsigned fadeProgress = min(progress, FADE_DURATION);
  unsigned effect2 = time / EFFECT_DURATION % EFFECT_COUNT;
  unsigned effect1 = effect2 == 0 ? EFFECT_COUNT - 1 : effect2 - 1;

  if (startup) {
    runEffect(strip, effects[effect2], 0, LED_COUNT, fadeProgress, TO_END);
    if (fadeProgress == FADE_DURATION) {
      startup = false;
    }
  } else {
    runEffect(strip, effects[effect1], 0, LED_COUNT, fadeProgress, FROM_START);
    runEffect(strip, effects[effect2], 0, LED_COUNT, fadeProgress, TO_END);
  }

#if SERIAL_TIMINGS == 1
  auto endTime = micros();
#endif

  strip.show();

#if SERIAL_TIMINGS == 1
  auto finalTime = micros();
  auto processTime = endTime - startTime;
  auto transferTime = finalTime - endTime;
  auto loopTime = finalTime - lastLoopTime;
  lastLoopTime = finalTime;
  Serial.print(processTime);
  Serial.print("\t");
  Serial.print(transferTime);
  Serial.print("\t");
  Serial.println(loopTime);
#endif
}
