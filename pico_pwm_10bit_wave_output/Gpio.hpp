#pragma once
#include <Arduino.h>
#include "pico/stdlib.h"

struct Gpio
{
  uint number;
  bool out, state = false;

  Gpio(uint number_, bool out_ = true) : number(number_), out(out_)
  {
    gpio_init(number);
    gpio_set_dir(number, out);
  }

  void set(bool x)
  {
    state = x;
    gpio_put(number, state);
  }

  bool get() { return gpio_get(number); }

  void toggle()
  {
    state = !state;
    set(state);
  }
};
