#pragma once

void setupPWM() {
  // PB1とPB2ピン（OC1AとOC1Bに相当）を出力モードに設定します。
  // これにより、PWM信号をこれらのピンから出力できます。
  DDRB |= (1 << PB1) | (1 << PB2);
  // TCCR1Aレジスタを設定します。
  // WGM11を1に設定することで、Fast PWMモード14（TOP=ICR1）の一部を有効にします。
  // COM1A1とCOM1B1を1に設定することで、非反転PWM出力をOC1AとOC1Bピンで有効にします。
  TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
  // TCCR1Bレジスタを設定します。
  // WGM12とWGM13を1に設定することで、Fast PWMモード14（TOP=ICR1）を完了させます。
  // CS10を1に設定することで、プリスケーラーを1に設定し、タイマーをシステムクロック（16MHz）で動作させます。
  TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS10);
  // ICR1レジスタにTOP値（255）を設定します。
  // f_PWM = f_clk_I/O / (N * (1 + TOP)) の式に基づき、
  // 16,000,000 / (1 * (1 + 255)) = 62,500Hzとなり、62.5kHzのPWM周波数が生成されます。
  ICR1 = 255;
}

void setupTimer2() {
  // Timer2をCTCモード (TOP=OCR2A) に設定
  TCCR2A = (1 << WGM21);
  // プリスケーラーを8に設定 (clk/8)
  TCCR2B = (1 << CS21);
  // 31.25kHzにするためのTOP値を設定
  // f_int = 16M / (8 * (1 + 63)) = 31250 Hz
  OCR2A = 63;  // 63 = 31250Hz, 127 = 15625Hz, 255 = 7812Hz
  // Compare Match A 割り込みを有効化
  TIMSK2 = (1 << OCIE2A);
}

void setupDac() {
  setupPWM();
  setupTimer2();
  sei();
  DDRC |= (1 << PORTC5);
}

int16_t process();

// Timer2のCompare Match A割り込みサービスルーチン
ISR(TIMER2_COMPA_vect) {
  PORTC |= _BV(PORTC5);  // FS Check

  uint16_t x = process() + 32768;

  OCR1A = x >> 8;
  OCR1B = x;

  PORTC &= ~_BV(PORTC5);  // FS Check
}
