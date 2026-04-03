#pragma once
#include <Arduino.h>

template <uint FIRST_PIN, int NUM_PINS, int NUM_SIDESET = 0>
struct PioInit
{
  PIO pio;
  uint sm, offset;
  pio_sm_config cfg;

  PioInit(const pio_program &program)
  {
    bool rc = pio_claim_free_sm_and_add_program_for_gpio_range(&program, &pio, &sm, &offset,
                                                               FIRST_PIN, NUM_PINS + NUM_SIDESET, true);
    hard_assert(rc);

    for (int i = 0; i < NUM_PINS + NUM_SIDESET; ++i)
      pio_gpio_init(pio, FIRST_PIN + i);

    pio_sm_set_consecutive_pindirs(pio, sm, FIRST_PIN, NUM_PINS + NUM_SIDESET, true);

    cfg = pio_get_default_sm_config();
    sm_config_set_wrap(&cfg, offset, offset + program.length + program.origin);

    sm_config_set_set_pins(&cfg, FIRST_PIN, NUM_PINS);

    if (NUM_SIDESET)
    {
      sm_config_set_sideset_pins(&cfg, FIRST_PIN + NUM_PINS);
      sm_config_set_sideset(&cfg, NUM_SIDESET, false, false);
    }

    pio_sm_init(pio, sm, offset, &cfg);
  }

  virtual void begin()
  {
    pio_sm_set_enabled(pio, sm, true);
  }

  void stop() { pio_sm_set_enabled(pio, sm, false); }

  void free()
  {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_unclaim(pio, sm);
  }
};
