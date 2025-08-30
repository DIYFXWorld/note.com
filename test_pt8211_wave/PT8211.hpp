#pragma once
#include <Arduino.h>
#include "hardware/pio.h"
#include "PT8211.pio.h"

// ex.
// const uint PT8211_PIN_BCK = 17; // ピン番号は連続させること
// const uint PT8211_PIN_WS = 18;
// const uint PT8211_PIN_DAT = 19;

struct PT8211
{
  PIO pio;
  uint sm, offset;
  const uint bck, ws, dat, pin_base;

  PT8211(uint BCK, uint WS, uint DAT) : bck(BCK), ws(WS), dat(DAT), pin_base(bck) {}

  // システムクロックとサンプリングレートからクロック分周を計算。
  void begin(uint sampling_rate)
  {
    bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&PT8211_program, &pio, &sm, &offset, pin_base, 3, true);
    hard_assert(success);

    pio_sm_config c = PT8211_program_get_default_config(offset);

    // clac clock division
    uint system_clock = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS) * 1000;
    uint bck_clock = sampling_rate * 16 /*bits per sample*/ * 2 /*channels*/ * 3;
    uint clock_div = system_clock / bck_clock;
    sm_config_set_clkdiv(&c, clock_div);

    pio_sm_set_consecutive_pindirs(pio, sm, pin_base, 3, true);
    sm_config_set_sideset_pins(&c, bck);
    sm_config_set_out_pins(&c, dat, 1);
    sm_config_set_out_shift(&c, false, true, 32);

    pio_gpio_init(pio, bck);
    pio_gpio_init(pio, ws);
    pio_gpio_init(pio, dat);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
  }

  void set(int16_t left, int16_t right)
  {
    pio_sm_put_blocking(pio, sm, (uint32_t)(left & 0xffff) << 16 | (uint32_t)(right & 0xffff));
  }
};