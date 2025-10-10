#pragma once
#include	<Arduino.h>

extern const PROGMEM int16_t WAVE_TABLE_SAW[256 + 1];
extern const PROGMEM int16_t WAVE_TABLE_SAW_REV[256 + 1];
extern const PROGMEM int16_t WAVE_TABLE_SQUARE[256 + 1];
extern const PROGMEM int16_t WAVE_TABLE_TRIANGLE[256 + 1];
extern const PROGMEM int16_t WAVE_TABLE_SIN[256 + 1];
extern const PROGMEM int16_t WAVE_TABLE_COS[256 + 1];

#define table_sin(x) getWaveTable(x, WAVE_TABLE_SIN)
#define table_cos(x) getWaveTable(x, WAVE_TABLE_COS)

inline int16_t getWaveTable(uint16_t i, const int16_t * table) {
  uint8_t decimal = i & 0xff;
  uint8_t idx = i >> 8;
  int16_t val1 = pgm_read_word(&(table[idx]));
  int16_t val2 = pgm_read_word(&(table[idx + 1]));
  return val1 + (((val2 - val1) * (int32_t)decimal) >> 8); // 掛け算するので32ビットへ拡張する
}