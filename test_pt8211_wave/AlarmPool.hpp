#pragma once
#include <cassert>
#include "hardware/timer.h"
#include "pico/time.h"

// AlarmPoolはPicoSDKで提供されるソフトウェアタイマー
// 1個のハードウェアタイマから最大255個の異なるタイマーコールバックを行える
// ソフトウェアタイマーなのでCPU時間とモリを消費する
struct AlarmPool
{
  alarm_pool_t *handle;
  repeating_timer_t *timer;
  int max_timer;

  // pool_numは使用するはハードウェアタイマーの番号で1,2,3のいずれか(0は止まる)
  // FreeRtosを使うならpool_numは0,1,2のいずれか(3は止まる)
  // max_numは登録できるタイマーの個数(255まで)
  AlarmPool(int pool_num, int max_num)
  {
    max_timer = max_num;
    handle = alarm_pool_create(pool_num, max_num);
    timer = new repeating_timer_t[max_num];
  }

  ~AlarmPool()
  {
    delete[] timer;
    if (handle != nullptr)
      alarm_pool_destroy(handle);
  }

  // timer_numは登録するタイマーの番号 (1なら0)
  void addCallbackUs(int timer_num, int64_t delay_us, repeating_timer_callback_t callback, void *user_data = nullptr)
  {
    assert(timer_num >= 0 && timer_num < max_timer);
    alarm_pool_add_repeating_timer_us(handle, -delay_us, callback, user_data, &timer[timer_num]);
  }

  void addCallbackMs(int timer_num, int32_t delay_ms, repeating_timer_callback_t callback, void *user_data = nullptr)
  {
    assert(timer_num >= 0 && timer_num < max_timer);
    alarm_pool_add_repeating_timer_ms(handle, -delay_ms, callback, user_data, &timer[timer_num]);
  }
};