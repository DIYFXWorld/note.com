#pragma once
#include <Arduino.h>
#include <hardware/pwm.h>
//
// PWMの割り込みを使った周期タイマー
// PWMを出力できるピンを指定しないと使えない。
// ピンを指定しても出力は出ないようにしている。
//
struct IntervalCallback
{
  uint sliceNum;

  IntervalCallback(uint dummy_gpio_num)
  {
    sliceNum = pwm_gpio_to_slice_num(dummy_gpio_num);
    pwm_set_irq_enabled(sliceNum, true);
    irq_set_enabled(PWM_IRQ_WRAP, true);
  }

  void begin(float interval_sec, void (*irq_handler)())
  {
    int wrap, clock_div;
    int system_clock_hz = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS) * 1000;
    getWrapClockDiv(system_clock_hz, interval_sec, wrap, clock_div);
    begin(wrap, clock_div, irq_handler);
  }

  void begin(int wrap, int clock_div, void (*irq_handler)())
  {
    pwm_set_clkdiv(sliceNum, clock_div);
    pwm_set_wrap(sliceNum, wrap);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, irq_handler);
    pwm_set_enabled(sliceNum, true);
  }

  void clearIrq() { pwm_clear_irq(sliceNum); }

  bool calculateTimerSettings(double system_clock_hz, double target_period_s, int &wrap, int &clock_div, double error)
  {
    //
    // I asked ChatGPT to write this.
    //
    const int MAX_TIMER_VALUE = 65535; // 汎用的なタイマーレジスタの最大値（例：16ビットタイマー）
    const int MAX_DIVIDER = 65535;     // 分周比の最大値（MCUによって異なるが、一般的に8ビット）

    double system_period_s = 1.0 / system_clock_hz; // システムクロックの周期を計算

    for (int divider = 1; divider <= MAX_DIVIDER; divider++) // 分周比を1から最大値まで順に試す
    {
      double wrap_double = (target_period_s / (system_period_s * divider)) - 1.0; // ラップ値を計算（浮動小数点数）

      if (wrap_double >= 0 && wrap_double <= MAX_TIMER_VALUE) // ラップ値が整数かつ許容範囲内かチェック
      {
        long long wrap_int = (long long)round(wrap_double);                // 浮動小数点誤差を考慮して最も近い整数に丸める
        double actual_period = system_period_s * divider * (wrap_int + 1); // 実際の周期を再計算

        if (fabs(actual_period - target_period_s) / target_period_s < error) // 実際の周期と目標周期が許容誤差内に収まっているかチェック
        {
          clock_div = divider;
          wrap = wrap_int;
          return true;
        }
      }
    }
    return false;
  }

  void getWrapClockDiv(double system_clock_hz, double target_period_s, int &wrap, int &clock_div)
  {
    double error = 0.000001;
    while (1)
    {
      if (calculateTimerSettings(system_clock_hz, target_period_s, wrap, clock_div, error))
        break;
      error *= 10;
      if (error > 10)
        break;
    }
  }
};