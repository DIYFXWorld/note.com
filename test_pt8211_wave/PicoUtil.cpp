#include  <Arduino.h>
#include  "PicoUtil.hpp"

void setSysClock(int freq /*MHz*/)
{
  set_sys_clock_khz(freq * KHZ, true);

  clock_configure(
      clk_peri,
      0,
      CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
      freq * MHZ,
      freq * MHZ);
}

int getPicoRevision()
{
  uint8_t x = *((uint8_t*)(SYSINFO_BASE+3));
  x = (0xf0 & x) >> 4;
  return x;
}

void printClocks()
{
  uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
  uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
  uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
  uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
  uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
  uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
  uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
  uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
#endif
  Serial.printf("pll_sys  = %dkHz\n", f_pll_sys);
  Serial.printf("pll_usb  = %dkHz\n", f_pll_usb);
  Serial.printf("rosc     = %dkHz\n", f_rosc);
  Serial.printf("clk_sys  = %dkHz\n", f_clk_sys);
  Serial.printf("clk_peri = %dkHz\n", f_clk_peri);
  Serial.printf("clk_usb  = %dkHz\n", f_clk_usb);
  Serial.printf("clk_adc  = %dkHz\n", f_clk_adc);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
  Serial.printf("clk_rtc  = %dkHz\n", f_clk_rtc);
#endif
}

void printHeap()
{
  size_t max = 1024 * 1024; // 1MBまで試す
  void* ptr = nullptr;
  while (max > 0) {
    ptr = malloc(max);
    if (ptr) {
      free(ptr);
      break;
    }
    max -= 1024; // 1KBずつ減らす
  }
  Serial.printf("Approx Free Heap: %u bytes\n", (unsigned int)max);
}
