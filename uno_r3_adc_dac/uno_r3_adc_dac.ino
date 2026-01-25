#include <Arduino.h>
#include "Audio.hpp"
#include "Util.hpp"
//
// サンプリングレート 31.25kHz
// 音声データ 符号付16ビット
// ※ADCの解像度が10ビットなので実質10ビット精度しかありませんが
// 6ビット分シフトしているので
// 計算は16ビット値(-32768 <= x <= 32767)として扱います。
// PWMキャリア 62.5kHz
//
// この関数の中で信号処理をします
int16_t processAudio(int16_t x) {
  return x;
}

void setup() {
  setupAudio();
}

void loop() {
}