#ifndef COLOR_H
#define COLOR_H

namespace Color {
  typedef uint32_t color;
  typedef uint8_t channel;

  // the progress type goes from 0 to 128
  // this is to make division much easier on the cpu since it's just a bit shift by 7
  // larger progress numbers would require a bigger integer
  typedef uint8_t progress;
  const progress MAX_PROGRESS = 128;
  const progress MIN_PROGRESS = 0;

  channel transitionChannel(const channel &c1, const channel &c2, const progress &progress) {
    if (c1 > c2) {
      return ((uint16_t)((c1 - c2) * (MAX_PROGRESS - progress)) / MAX_PROGRESS) + c2;
    } else {
      return ((uint16_t)((c2 - c1) * progress) / MAX_PROGRESS) + c1;
    }
  }

  color transition(const color &c1, const color &c2, const progress &progress) {
    auto c1s = reinterpret_cast<const channel(&)[4]>(c1);
    auto c2s = reinterpret_cast<const channel(&)[4]>(c2);
    channel result[4] = {
      transitionChannel(c1s[0], c2s[0], progress),
      transitionChannel(c1s[1], c2s[1], progress),
      transitionChannel(c1s[2], c2s[2], progress),
      0, // ignore white channel
    };
    return reinterpret_cast<color&>(result);
  }
}

#endif
