#pragma once
#include <Arduino.h>
#include "PotReader.pio.h"
#include "PioInit.hpp"
/* schematic
　　　　　　　　　　　　　 1                 3　　　　　
GPIO ----- 470 ohm --+-- 10kB Potentiometer --+-- 3.3V
                     |           |2           |
                     |           --------------
                   0.1uF
                     |
                    GND
*/

template <uint FIRST_PIN, uint NUM_PINS = 1>
struct PotReader : public PioInit<FIRST_PIN, NUM_PINS>
{
  using super = PioInit<FIRST_PIN, NUM_PINS>;

  uint32_t MAX_COUNT;
  uint32_t result[NUM_PINS];

  PotReader() : super(PotReader_program)
  {
  }

  void begin() override
  {
    // 追加の設定
    for (int i = 0; i < NUM_PINS; ++i)
      pinMode(FIRST_PIN + i, INPUT_PULLUP);

    float clock = clock_get_hz(clk_sys);
    float div = clock / 50'000'000; // 50MHzに分周。クロックを上げても精度は上がらないので、低い方に合わせる。
    MAX_COUNT = clock * 0.001f;     // 1ms
    sm_config_set_clkdiv(&(super::cfg), div);
    // start
    super::begin();
  }

  void discharge(uint pin)
  {
    gpio_disable_pulls(pin);
    gpio_set_dir(pin, GPIO_OUT); // 出力にする
    gpio_put(pin, LOW);          // LOWにする
    sleep_us(300);               // 放電しきるまで待つ
  }

  uint32_t get(uint pin)
  {
    // --- 1. PIOにピンの制御権を渡す ---
    pio_gpio_init(super::pio, pin);
    sm_config_set_in_pins(&(super::cfg), pin);
    sm_config_set_jmp_pin(&(super::cfg), pin);

    // ピンを入力に設定
    gpio_set_dir(pin, GPIO_IN);

    // --- 2. SMを初期化して有効化 ---
    pio_sm_init(super::pio, super::sm, super::offset, &(super::cfg));
    pio_sm_set_enabled(super::pio, super::sm, true);

    // --- 3. 計測開始 ---
    // カウンタ初期値を送る。PIOの pull block で止まっていたのが動き出す
    pio_sm_put_blocking(super::pio, super::sm, MAX_COUNT);

    // --- 4. 結果の取得 ---
    uint32_t remaining_count = pio_sm_get_blocking(super::pio, super::sm);

    pio_sm_set_enabled(super::pio, super::sm, false);

    // --- 5. 後処理 ---
    // 次回の discharge() (gpio_put) が効くように、GPIO制御に戻す
    gpio_init(pin);

    // カウントダウンなので「消費したカウント」を計算して返す
    if (remaining_count > MAX_COUNT)
      return MAX_COUNT; // アンダーフロー対策
    return (MAX_COUNT - remaining_count);
  }

  uint read()
  {
    for (int i = 0; i < NUM_PINS; ++i)
    {
      uint pin = FIRST_PIN + i;
      discharge(pin);
      result[i] = get(pin);
    }
    return result[0];
  }

  uint operator[](int index) const
  {
    if (index < 0 || index >= NUM_PINS)
      return 0; // 範囲外アクセス対策
    return result[index];
  }
};
