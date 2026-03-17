//
// RaspberryPico用
// Dual PWM Dac 16ビット波形出力プログラム
// 実質的な精度は12～14ビット
// キャリアが高いので簡易RCフィルタでもまあまあな波形。
//
#define _USE_MATH_DEFINES
#include "IntervalCallback.hpp"
#include "DualPwmDac.hpp"
#include "Oscillator.hpp"
#include "Gpio.hpp"
#include "PicoUtil.hpp"
#include "SoftwarePwm.hpp"
//
// Raspberry pico wave output test
// 16bit wave table
//
const uint NEW_SYSTEM_CLOCK = 250'000'000;
const uint SAMPLING_RATE = 48'000;  // サンプリングレート

const uint PWM_OUTPUT_PIN_HIGH = 14;         // 3.9k ohm
const uint PWM_OUTPUT_PIN_LOW = 15;          // 1M ohm
const uint DUMMY_INTERVAL_CALLBACK_PIN = 5;  // unused pin number

DualPwmDac pwmDac(PWM_OUTPUT_PIN_HIGH, PWM_OUTPUT_PIN_LOW);
IntervalCallback intervalCallback(DUMMY_INTERVAL_CALLBACK_PIN);

Gpio gpio0(0);  // sampling late output

Oscillator oscSin(WAVE_TABLE_SIN, SAMPLING_RATE);
Oscillator oscSaw(WAVE_TABLE_SAW, SAMPLING_RATE);
Oscillator oscTri(WAVE_TABLE_TRIANGLE, SAMPLING_RATE);
Oscillator oscSquare(WAVE_TABLE_SQUARE, SAMPLING_RATE);
SoftwarePwm oscPwm(SAMPLING_RATE);

OscillatorBase *oscArray[5] = { &oscSin, &oscSaw, &oscTri, &oscSquare, &oscPwm };
OscillatorBase *osc;

int oscIndex = 0;
uint count = 0;

void on_pwm_wrap() {
  intervalCallback.clearIrq();

  gpio0.set(HIGH);  // sampling rate

  if (osc)
    pwmDac.set(osc->get());

  if (++count == SAMPLING_RATE * 2) {
    count = 0;
    oscIndex = ++oscIndex % 5;
    osc = oscArray[oscIndex];
    osc->setFreq(1000);
  }

  gpio0.set(LOW);  // sampling rate
}

void setup() {
  setSysClock(NEW_SYSTEM_CLOCK);
  pwmDac.begin();
  intervalCallback.begin(1.0 / SAMPLING_RATE, on_pwm_wrap);
}

void loop() {
}
