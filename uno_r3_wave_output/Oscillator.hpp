#pragma once
#include <fixed.h>
#include "WaveTable.hpp"

struct Oscillator {
private:
  int fs;
  fixed8 theta;
  fixed8 thetaStep;
  const int16_t *table;
  const fixed8 F65535 = fixed8(65535);

public:
  Oscillator(const int16_t *tbl, int _fs)
    : fs(_fs), theta(0.f), thetaStep(0.f), table(tbl) {
    setFreq(1000);
  }

  void setFreq(float freq /*Hz*/) {
    thetaStep = fixed8(freq * 65535.f / fs);
  }

  int16_t get() {
    int16_t x = getWaveTable(theta.toInt(), table);
    theta += thetaStep;
    if (theta > F65535)
      theta -= F65535;
    return x;
  }

  void Reset() {
    theta = 0;
  }
};