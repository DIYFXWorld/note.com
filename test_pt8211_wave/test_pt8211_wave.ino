#include <Arduino.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "PT8211.hpp"
#include "IntervalCallback.hpp"
#include "Gpio.hpp"
#include "PicoUtil.hpp"
#include "Oscillator.hpp"
#include "SoftwarePwm.hpp"

const uint SAMPLING_RATE = 40'000;
const uint PT8211_PIN_BCK = 16;  // ピン番号は連続させること
const uint PT8211_PIN_WS = 17;
const uint PT8211_PIN_DAT = 18;

IntervalCallback intervalCallback(5);
PT8211 dac(PT8211_PIN_BCK, PT8211_PIN_WS, PT8211_PIN_DAT);
Gpio gp0(0);            // sampling rate
Gpio led(LED_BUILTIN);  // indicator

Oscillator oscSin(WAVE_TABLE_SIN, SAMPLING_RATE);
Oscillator oscSaw(WAVE_TABLE_SAW, SAMPLING_RATE);
Oscillator oscSawRev(WAVE_TABLE_SAW_REV, SAMPLING_RATE);
Oscillator oscTri(WAVE_TABLE_TRIANGLE, SAMPLING_RATE);
Oscillator oscSquare(WAVE_TABLE_SQUARE, SAMPLING_RATE);
SoftwarePwm oscPwm(SAMPLING_RATE);

OscillatorBase *oscArray[6] = { &oscSin, &oscSaw, &oscSawRev, &oscTri, &oscSquare, &oscPwm };
OscillatorBase *osc;

int oscIndex = 0;
uint count = 0;

inline int16_t sat(int x) {
  if (x > 32767) return 32767;
  if (x < -32768) return -32768;
  return x;
}

void __not_in_flash_func(callbackFunc)() {
  intervalCallback.clearIrq();
  gp0.set(HIGH);  // sampling rate & core utilization rate

  if (osc) {
    int x0 = osc->get();
    int x1 = sat(-x0);  // int16_t型の-32768を反転させるとオーバーフローすることに注意
                        // intへ拡張して丸める
    dac.set(x0, x1);
  }

  if (++count == SAMPLING_RATE * 2) {
    count = 0;
    oscIndex = ++oscIndex % 6;
    osc = oscArray[oscIndex];
    osc->setFreq(1000);
  }

  gp0.set(LOW);  // sampling rate & core utilization rate
}

void setup() {
  setSysClock(250);
  //  Serial.begin(115200);
  //  while(!Serial)
  ;
  //  delay(2000);
  led.set(true);
  // DACの設定
  dac.begin(SAMPLING_RATE);  // システムクロックとサンプリングレートからクロック分周を計算。
  // タイマーの設定
  intervalCallback.begin(1.0 / SAMPLING_RATE, callbackFunc);
}

void loop() {}
