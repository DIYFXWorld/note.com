#pragma once
#include <stdint.h>

struct OscillatorBase
{
  virtual int16_t get() = 0;
  virtual void setFreq(float Hz) = 0;
  virtual void Reset() {}
  virtual void setLfoFreq(float Hz) {}
  virtual void setLfoDepth(float Hz) {}
};