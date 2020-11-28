#include "avr/io.h"
#include "avr/interrupt.h"

#define PORT_LEDA     PORTD5
#define DD_LEDA       DDD5

#define PORT_LEDB     PORTD6
#define DD_LEDB       DDD6

#define PORT_PATTERN  PORTD2
#define DD_PATTERN    DDD2
#define PIN_PATTERN   PIND2

#define PORT_MODE     PORTD3
#define DD_MODE       DDD3
#define PIN_MODE      PIND3

volatile uint8_t flags = 0;
#define F_PATTERN_CHANGED 7
#define F_TIMER0_OVF      6
#define F_ADC_DONE        5
#define F_LED_STATE       4

volatile uint8_t adc_val = 0;

// pattern switch ISR
ISR(INT0_vect) {
  flags |= _BV(F_PATTERN_CHANGED);
}

// timer 0 ISR
ISR(TIMER0_OVF_vect) {
  flags |= _BV(F_TIMER0_OVF);
  flags ^= _BV(F_LED_STATE);
}

// ADC done ISR
ISR(ADC_vect) {
  flags |= _BV(F_ADC_DONE);
  adc_val = ADCH;
}

int main(void) {
  // set up inputs/outputs
  // main LEDs: output, OC0A/OC0B (PD5/PD6)
  // pattern switch (blink/fade): input, pullup, INT0 (PD2)
  // mode switch (steady/pattern): input, no pullup, INT1 (PD3)
  DDRD = _BV(DD_LEDA) | _BV(DD_LEDB);
  PORTD = _BV(PORT_PATTERN);

  // turn on what we need, turn off what we don't
  //  - Timer 0: output PWM
  //  - Timer 1: blink/fade delay timing
  //  - ADC: read delay time/brightness potentiometer
  PRR = ~_BV(PRTIM0) & ~_BV(PRTIM1) & ~_BV(PRADC);

  // enable interrupts for timers, switches, ADC, etc.
  EICRA = _BV(ISC00);
  EIMSK = _BV(INT0);
  asm("sei");

  // set up ADC to take continuous readings on ADC0 against AREF
  // and left adjust the result
  ADMUX |= _BV(ADLAR);
  ADCSRA |= _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  ADCSRA |= _BV(ADEN);
  ADCSRA |= _BV(ADSC);

  // set up PWM on timer 0
  // fast PWM, 256 bit resolution
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  // enable interrupts
  TIMSK0 = _BV(TOIE0);
  // clear compare regs
  OCR0A = 0x00;
  OCR0B = 0x00;

  // this only works b/c the steady/pattern switch is also
  // the power switch (SPDT w/ off position)
  if( PIND & _BV(PIN_MODE) ) {
    // We're splitting time between the LED strings - effectively, each string
    // does one on/off cycle then spends an entire PWM period off while the
    // other string does its on/off cycle. This effectively halves the PWM
    // frequency, meaning we need really fast PWM!
    // no prescaling (@ 8MHz sys, 8MHz tick, 31.25kHz PWM freq,
    // 15.625kHz effective PWM freq)
    TCCR0B = _BV(CS00);

    while( 1 ) {
      // read ADC to get brightness
      if( flags & _BV(F_ADC_DONE) ) {
        flags &= ~_BV(F_ADC_DONE);
        OCR0A = adc_val;
        OCR0B = adc_val;
      }

      if( flags & _BV(F_TIMER0_OVF) ) {
        flags &= ~_BV(F_TIMER0_OVF);
        // LED outputs CANNOT be on simultaneously
        TCCR0A &= ~_BV(COM0A1) & ~_BV(COM0A0) & ~_BV(COM0B1) & ~_BV(COM0B0);
        if( flags & _BV(F_LED_STATE) ) {
          TCCR0A |= _BV(COM0A1);
        } else {
          TCCR0A |= _BV(COM0B1);
        }
      }
    }

  } else {
    // This is sticking to one string at a time (they will be switching, but
    // slow enough that flickering isn't a problem). Therefore, we can use
    // slower PWM.
    // sys/8 (@ 8MHz sys, 1MHz tick, ~3.9kHz PWM freq)
    TCCR0B = _BV(CS01);

    while( 1 ) {
      // read ADC to get pattern speed
      if( flags & _BV(F_ADC_DONE) ) {
        flags &= ~_BV(F_ADC_DONE);
      }

      if( PIND &  PIN_PATTERN ) {
        // blink mode

      } else {
        // flash mode
      }
    }
  }

  // we should never get here!
  return 0;
}
