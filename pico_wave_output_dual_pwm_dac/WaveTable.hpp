#pragma once
#include <cstdint>

extern const int16_t WAVE_TABLE_SAW[256 + 1];
extern const int16_t WAVE_TABLE_SAW_REV[256 + 1];
extern const int16_t WAVE_TABLE_SQUARE[256 + 1];
extern const int16_t WAVE_TABLE_TRIANGLE[256 + 1];
extern const int16_t WAVE_TABLE_SIN[256 + 1];
extern const int16_t WAVE_TABLE_COS[256 + 1];

#define table_sin(x) getWaveTable(x, WAVE_TABLE_SIN)
#define table_cos(x) getWaveTable(x, WAVE_TABLE_COS)

inline int16_t getWaveTable(int i, const int16_t *const table) // i = 0...65535
{
	int decimal = i & 0xff;
	int idx = i >> 8;
	return table[idx] + (((table[idx + 1] - table[idx]) * decimal) >> 8);
}