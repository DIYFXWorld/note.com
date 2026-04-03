#pragma once
#include <Arduino.h>

inline uint getSecToClockCycles(float sec) // seconds to clock cycles
{
  return clock_get_hz(clk_sys) * sec;
}

inline void setSysClockMhz(int freq_mhz)
{
  int freq_khz = freq_mhz * 1000;
  set_sys_clock_khz(freq_khz, true);
  clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, freq_khz, freq_khz);
}
