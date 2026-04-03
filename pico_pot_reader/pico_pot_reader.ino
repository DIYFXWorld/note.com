#include <Arduino.h>
#include "PotReader.hpp"
#include "Gpio.hpp"
#include "MovingAverage.hpp"
#include "Median9.hpp"
/* schematic
　　　　　　　　　　　　　 1                 3　　　　　
GPIO ----- 470 ohm --+-- 10kB Potentiometer --+-- 3.3V
                     |           |2           |
                     |           --------------
                   0.1uF
                     |
                    GND
*/

PotReader<0, 4> pots0;  // 最初のピン番号と、使用するピンの数
                        // ピンは連続している必要がある。
                        // 例えば、0, 1, 2, 3を使うなら、PotReader<0,4>
                        // 2, 3, 4を使うなら、GpioAdc<2,3>
PotReader<4, 4> pots1;  // 別のグループを使うこともできる。例えば、4, 5, 6, 7 を使うならPotReader<4,4>
                        // 謎仕様により1グループは7個まで
// 平滑フィルタ
// Median9 中央値
// MivingAverage 平均化
// フィルタを強くするほどブレは少なくなりますが、応答速度は悪くなります。
Median9 mid0, mid1, mid2, mid3, mid4, mid5, mid6, mid7;
MovingAverage<int, 50> avg0, avg1, avg2, avg3, avg4, avg5, avg6, avg7;

Gpio intervalOutPin(10);  // ピン番号

void setup() {
  Serial.begin(115200);
  pots0.begin();
  pots1.begin();
}

int count = 0;

void loop() {
  intervalOutPin.toggle();  // ピンをトグルさせて大雑把な時間を測る

  pots0.read();
  pots1.read();

  Serial.printf("%u %u %u %u %u %u %u %u\n",
                mid0(avg0(pots0[0])) / 152,  // 100ステップへ変換
                mid1(avg1(pots0[1])) / 160,  // 除数はコンデンサと抵抗によって変わるので
                mid2(avg2(pots0[2])) / 178,  // 全て違う値になります
                mid3(avg3(pots0[3])) / 154,
                mid4(avg4(pots1[0])) / 168,
                mid5(avg5(pots1[1])) / 140,
                mid6(avg6(pots1[2])) / 153,
                mid7(avg7(pots1[3])) / 145);
}