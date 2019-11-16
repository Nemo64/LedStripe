#ifndef COLOR_H
#define COLOR_H

namespace Color {
typedef uint32_t color_t;
typedef uint8_t channel_t;

// the progress type goes from 0 to 128
// this is to make division much easier on the cpu since it's just a bit shift by 7
// larger progress numbers would require a bigger integer
typedef uint8_t progress_t;
const progress_t MAX_PROGRESS = 128;
const progress_t MIN_PROGRESS = 0;

inline channel_t transitionChannel(channel_t c1, channel_t c2, progress_t progress) {
  return c1 + (c2 - c1) * progress / MAX_PROGRESS;
}

inline color_t transition(color_t c1, color_t c2, progress_t progress) {
  auto c1s = reinterpret_cast<const channel_t(&)[4]>(c1);
  auto c2s = reinterpret_cast<const channel_t(&)[4]>(c2);
  channel_t result[4] = {
    transitionChannel(c1s[0], c2s[0], progress),
    transitionChannel(c1s[1], c2s[1], progress),
    transitionChannel(c1s[2], c2s[2], progress),
    0, // ignore white channel
  };
  return reinterpret_cast<color_t&>(result);
}
}

#endif
