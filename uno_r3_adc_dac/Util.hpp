#pragma once

// clang-format off
#define NOP10() asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n");asm("nop \n")
#define NOP100() NOP10();NOP10();NOP10();NOP10();NOP10();NOP10();NOP10();NOP10();NOP10();NOP10()
#define NOP500() NOP100();NOP100();NOP100();NOP100();NOP100()
#define NOP1000() NOP500();NOP500()
// clang-format on
