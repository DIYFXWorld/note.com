//
// 重要:fixedライブラリが必要なのでArduinoIDEからインストールしてください
// "The fixed library is required, so please install it in your Arduino IDE."
//
// Arduino Uno R3
//
// Dual PWM 16ビット波形出力
// Dual PWM 16bit wave output
//
#include <Arduino.h>
#include "Dac.hpp"
#include "Oscillator.hpp"

const int SAMPLING_RATE = 31250;  // sampling rate

Oscillator oscSin(WAVE_TABLE_SIN, SAMPLING_RATE);
Oscillator oscTri(WAVE_TABLE_TRIANGLE, SAMPLING_RATE);
Oscillator oscSaw(WAVE_TABLE_SAW, SAMPLING_RATE);
Oscillator oscSawRev(WAVE_TABLE_SAW_REV, SAMPLING_RATE);
Oscillator oscSqu(WAVE_TABLE_SQUARE, SAMPLING_RATE);

Oscillator* osc = &oscSin;

// この関数は31.25kHzで実行されます
int16_t process() {
  return osc->get();  // -32768...32767を返す
}

void setup() {
  setupDac();
}

int c = 0;
void loop() {
  // clang-format off
  if(c == 0) osc = &oscSin;
  if(c == 1) osc = &oscSaw;
  if(c == 2) osc = &oscSawRev;
  if(c == 3) osc = &oscTri;
  if(c == 4) osc = &oscSqu;
  // clang-format on
  osc->setFreq(1000);
  c = ++c % 5;
  delay(3000);
}