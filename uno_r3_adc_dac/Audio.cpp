#include <Arduino.h>
#include "Audio.hpp"

#define PWM_FREQ (0x00FF + 1) // pwm frequency - see table

void setupAudio()
{
  // ADC
  ADMUX = 0x60;
  ADCSRA = 0xe5;
  ADCSRB = 0x07;
  DIDR0 = 0x01;
  // Timer1 (ADC clock source)
  TCCR1A = (1 << WGM11);
  TCCR1B = 0x11;
  TIMSK1 = 0x20;
  ICR1H = (PWM_FREQ >> 8);
  ICR1L = (PWM_FREQ & 0xff);
  DDRB |= 0x02;
  // Timer2 (Dual PWM DAC)
  DDRB |= (1 << PB3);
  DDRD |= (1 << PD3);
  TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2A1) | (1 << COM2B1);
  TCCR2B = (1 << CS20);
  // turn on interrupt
  sei();
  DDRC |= _BV(PORTC5); // FS check pin;
}

ISR(TIMER1_CAPT_vect)
{
  PORTC |= _BV(PORTC5); // FS check

  uint16_t PCM = ADCL | (ADCH << 8);

  PCM = processAudio(PCM + 32768) - 32768;

  // Dual PWM DAC
  OCR2A = PCM >> 8;
  OCR2B = PCM;

  PORTC &= ~_BV(PORTC5); // FS check
}
