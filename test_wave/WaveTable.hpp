#pragma once
#include <cstdint>

extern const int16_t WAVE_TABLE_SAW[256 + 1];
extern const int16_t WAVE_TABLE_OCT_SAW[256 + 1];
extern const int16_t WAVE_TABLE_SQUARE[256 + 1];
extern const int16_t WAVE_TABLE_FM1[256 + 1];
extern const int16_t WAVE_TABLE_FM2[256 + 1];
extern const int16_t WAVE_TABLE_FM3[256 + 1];
extern const int16_t WAVE_TABLE_TRIANGLE[256 + 1];
extern const int16_t WAVE_TABLE_SIN[256 + 1];

//#define table_cos(x) getWaveTable(x, WAVE_TABLE_COS)
#define table_sin(x) getWaveTable(x, WAVE_TABLE_SIN)
//#define mod_sin16(x) getWaveTable(x, WAVE_TABLE_MOD_SIN)
//#define triangle16(x) getWaveTable(x, WAVE_TABLE_TRIANGLE)
//#define saw16(x) getWaveTable(x, WAVE_TABLE_SAW)
//#define saw_reverse16(x) getWaveTable(x, SAW_REV_TABLE)
//#define square16(x) getWaveTable(x, SQU_TABLE)

inline int16_t getWaveTable(int i, const int16_t *const table) // i = 0...65535
{
	int decimal = i & 0xff;
	int idx = i >> 8;
	return table[idx] + (((table[idx + 1] - table[idx]) * decimal) >> 8);
}
