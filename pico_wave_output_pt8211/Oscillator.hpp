#pragma once
#include <cmath>
#include "OscillatorBase.hpp"
#include "WaveTable.hpp"

struct Oscillator : public OscillatorBase
{
private:
  int fs;
  float theta;
  float thetaStep;
  const int16_t *table;

public:
  Oscillator(const int16_t *tbl, int _fs)
      : fs(_fs), theta(0.f),
        thetaStep(0.f), table(tbl)
  {
    setFreq(1000);
  }

  void setFreq(float Hz)
  {
    thetaStep = Hz * 65535.f / fs;
  }

  int16_t get()
  {
    int16_t x = getWaveTable((int)theta, table);
    theta = fmod(theta + thetaStep, 65535.f);
    return x;
  }

  enum
  {
    D90 = 65535 / 4,
    D180 = 65535 / 2,
    D120 = 65535 / 3,
    D240 = 65535 * 2 / 3,
    D270 = 65535 * 3 / 4,
  };

  int get(const int &v)
  {
    int t = theta + v;
    t %= 65535;
    return getWaveTable(t, table);
  }

  void Reset() { theta = 0; }

  void setTable(const int16_t *t) { table = t; }
};