#pragma once
#include <Arduino.h>
#include <hardware/pwm.h>

struct PwmDac
{
  uint sliceNum;
  int16_t resolution;
  int16_t zero;
  int16_t maxValue;

  PwmDac(uint gpio_num, uint16_t resolution_) : resolution(resolution_)
  {
    zero = resolution / 2;
    maxValue = resolution - 1;
    gpio_set_function(gpio_num, GPIO_FUNC_PWM);
    sliceNum = pwm_gpio_to_slice_num(gpio_num);
    pwm_set_clkdiv(sliceNum, 1);
    pwm_set_wrap(sliceNum, resolution - 1);
  }

  void begin() { pwm_set_enabled(sliceNum, true); }

  void set(int16_t x)
  {
    x += zero;
    if (x > maxValue)
      x = maxValue;
    else if (x < 0)
      x = 0;
    pwm_set_chan_level(sliceNum, PWM_CHAN_A, x);
  }
};
