#pragma once

struct DualPwmDac
{
  int pinHigh, pinLow;
  uint sliceNum;

  DualPwmDac(int pin_high, int pin_low)
      : pinHigh(pin_high), pinLow(pin_low) {}

  void begin()
  {
    gpio_set_function(pinHigh, GPIO_FUNC_PWM);
    gpio_set_function(pinLow, GPIO_FUNC_PWM);

    sliceNum = pwm_gpio_to_slice_num(pinHigh);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 1.f);
    pwm_config_set_wrap(&config, 255);
    pwm_init(sliceNum, &config, true);
  }

  void set(int16_t x) // -32768 <= x <= 32767
  {
    uint16_t u = x + 32768;
    pwm_set_both_levels(sliceNum, (u >> 8) & 0xFF, u & 0xFF);
  }
};
