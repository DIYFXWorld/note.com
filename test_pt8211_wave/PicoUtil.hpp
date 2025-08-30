#pragma once

void setSysClock(int freq /*MHz*/);

int getPicoRevision();

void printClocks();
void printHeap();

// clang-format off
#define	NOP()					asm volatile ("nop")
#define	NOP_5()			  { NOP();NOP();NOP();NOP();NOP(); }
#define	NOP_10()			{ NOP_5();NOP_5(); }
#define	NOP_20()			{ NOP_10();NOP_10(); }
#define	NOP_30()			{ NOP_10();NOP_20(); }
#define	NOP_40()			{ NOP_20();NOP_20(); }
#define	NOP_50()			{ NOP_20();NOP_30(); }
#define	NOP_60()			{ NOP_30();NOP_30(); }
#define	NOP_70()			{ NOP_40();NOP_30(); }
#define	NOP_80()			{ NOP_40();NOP_40(); }
#define	NOP_90()			{ NOP_50();NOP_40(); }
#define	NOP_100()			{ NOP_50();NOP_50(); }
#define	NOP_200()			{ NOP_100();NOP_100(); }
#define	NOP_300()			{ NOP_100();NOP_200(); }
#define	NOP_400()			{ NOP_200();NOP_200(); }
#define	NOP_500()			{ NOP_200();NOP_300(); }
#define	NOP_600()			{ NOP_300();NOP_300(); }
#define	NOP_700()			{ NOP_400();NOP_300(); }
#define	NOP_800()			{ NOP_400();NOP_400(); }
#define	NOP_900()			{ NOP_500();NOP_400(); }
#define	NOP_1000()		{ NOP_500();NOP_500(); }
#define	NOP_2000()		{ NOP_1000();NOP_1000(); }
#define	NOP_3000()		{ NOP_2000();NOP_1000(); }
#define	NOP_4000()		{ NOP_2000();NOP_2000(); }
#define	NOP_5000()		{ NOP_2000();NOP_3000(); }
#define	NOP_6000()		{ NOP_3000();NOP_3000(); }
#define	NOP_7000()		{ NOP_4000();NOP_3000(); }
#define	NOP_8000()		{ NOP_4000();NOP_4000(); }
#define	NOP_9000()		{ NOP_5000();NOP_4000(); }
#define	NOP_10000()		{ NOP_5000();NOP_5000(); }
#define	NOP_20000()		{ NOP_10000();NOP_10000(); }
#define	NOP_30000()		{ NOP_10000();NOP_20000(); }
#define	NOP_40000()		{ NOP_20000();NOP_20000(); }
#define	NOP_50000()		{ NOP_20000();NOP_30000(); }
// clang-format on
