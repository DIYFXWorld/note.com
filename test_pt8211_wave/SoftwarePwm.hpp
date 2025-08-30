#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include "OscillatorBase.hpp"
#include "WaveTable.hpp"

struct SoftwarePwm : public OscillatorBase
{
  int fs;
  float carrier_phase = 0.f;
  float lfo_phase = 0.f;
  float carrier_phase_increment;
  float lfo_phase_increment;
  float lfo_depth = 0.45f;

  SoftwarePwm(int fs_) : fs(fs_)
  {
    setFreq(1000.f);
    setLfoFreq(1.f);
  }

  void setFreq(float freq) { carrier_phase_increment = freq / fs; }
  void setLfoFreq(float freq) { lfo_phase_increment = freq / fs; }
  void setLfoDepth(float depth) { lfo_depth = depth; }

  int16_t get()
  {
    float lfo_value = table_sin(lfo_phase * 65535) / 32767.f;

//    float lfo_value = sin(2.f * M_PI * lfo_phase);
    float duty_cycle = 0.5f + lfo_value * lfo_depth;

    // 3. PWM波形を生成
    // 位相がデューティ比より小さければON、そうでなければOFF
    int16_t sample = (carrier_phase < duty_cycle) ? 32767 : -32768;

    // 5. 位相カウンターを更新
    carrier_phase += carrier_phase_increment;
    lfo_phase += lfo_phase_increment;

    // 位相が1.0を超えたらリセット
    if (carrier_phase > 1.f)
      carrier_phase -= 1.f;
    if (lfo_phase > 1.f)
      lfo_phase -= 1.f;

    return sample;
  }
};
