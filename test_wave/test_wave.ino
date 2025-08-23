#define _USE_MATH_DEFINES
#include <cmath>
#include "IntervalCallback.hpp"
#include "PwmDac.hpp"
#include "Oscillator.hpp"
#include "Gpio.hpp"
#include "PicoUtil.hpp"
#include "SoftwarePwm.hpp"
//
// Raspberry pico wave output test
// 10bit wave table
//
const uint SYSTEM_CLOCK = 250'000'000;
const uint SAMPLING_RATE = 32'000;
const uint PWM_DAC_RESOLUTION = 1024;

const uint PWM_OUTPUT_PIN = 2;
const uint DUMMY_INTERVAL_CALLBACK_PIN = 5;  // unused pin number

PwmDac pwmDac(PWM_OUTPUT_PIN, PWM_DAC_RESOLUTION);
IntervalCallback intervalCallback(DUMMY_INTERVAL_CALLBACK_PIN);

Gpio gpio0(0);  // sampling rate output

Oscillator oscSin(WAVE_TABLE_SIN, SAMPLING_RATE);
Oscillator oscSaw(WAVE_TABLE_SAW, SAMPLING_RATE);
Oscillator oscTri(WAVE_TABLE_TRIANGLE, SAMPLING_RATE);
Oscillator oscSquare(WAVE_TABLE_SQUARE, SAMPLING_RATE);
SoftwarePwm oscPwm(SAMPLING_RATE);

OscillatorBase *oscArray[5] = { &oscSin, &oscSaw, &oscTri, &oscSquare, &oscPwm };
OscillatorBase *osc = &oscSin;

int oscIndex = 0;
uint count;

void on_pwm_wrap() {
  intervalCallback.clearIrq();
  gpio0.set(HIGH);
  pwmDac.set(osc->get());

  if (++count == SAMPLING_RATE * 2) {
    count = 0;
    oscIndex = ++oscIndex % 5;
    osc = oscArray[oscIndex];
  }

  gpio0.set(LOW);
}

void setup() {
  setSysClock(SYSTEM_CLOCK);
  pwmDac.begin();
  intervalCallback.begin(1.0 / SAMPLING_RATE, on_pwm_wrap);
}

void loop() {
}
