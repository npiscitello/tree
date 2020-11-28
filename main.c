#include "avr/io.h"
#include "avr/interrupt.h"

// all tick calcs assume 8MHz clock

// min/max number of ticks for timer1 (7812.5Hz)
#define DELAY_MIN   586     // 0.75 sec/reset
#define DELAY_MAX   15625   // 2 sec/reset

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
#define F_LED_BANK        5
#define F_TIMER1_TRIG     4
#define F_ADC_DONE        3

volatile uint8_t bright_val = 0;
volatile uint8_t delay_val = 0;
volatile uint8_t* adc_reg = &bright_val;

// timer 0 ISR - set flag for pattern/steady loops
ISR(TIMER0_OVF_vect) {
  flags |= _BV(F_TIMER0_OVF);
}

// timer 1 ISR - set flag for pattern loop
ISR(TIMER1_COMPA_vect) {
  flags |= _BV(F_TIMER1_TRIG);
}

// timer 2 ISR - swap ADC regs and trigger an ADC conversion
ISR(TIMER2_COMPA_vect) {
  if( adc_reg == &bright_val ) {
    adc_reg = &delay_val;
    ADMUX = (ADMUX & 0xF0) | _BV(MUX0);
  } else {
    adc_reg = &bright_val;
    ADMUX = (ADMUX & 0xF0);
  }
  ADCSRA |= _BV(ADSC);
}

// ADC done ISR - write out result
ISR(ADC_vect) {
  flags |= _BV(F_ADC_DONE);
  *adc_reg = ADCH;
}

void set_led_bank( uint8_t led_bank ) {
  // LED outputs CANNOT be on simultaneously
  TCCR0A &= ~_BV(COM0A1) & ~_BV(COM0A0) & ~_BV(COM0B1) & ~_BV(COM0B0);
  if( led_bank ) {
    TCCR0A |= _BV(COM0A1);
  } else {
    TCCR0A |= _BV(COM0B1);
  }
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
  //  - Timer 2: ADC management
  //  - ADC: read delay time/brightness potentiometer
  PRR = ~_BV(PRTIM0) & ~_BV(PRTIM1) & ~_BV(PRTIM2) & ~_BV(PRADC);

  // enable interrupts for timers and ADC
  asm("sei");

  // set up ADC in oneshot mode and left adjust the result
  ADMUX = _BV(ADLAR);
  // sys/64, 125kHz tick
  ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  ADCSRA |= _BV(ADEN);

  // set up PWM on timer 0
  // fast PWM, 256 bit resolution
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  // clear compare regs (LEDs off)
  OCR0A = 0x00;
  OCR0B = 0x00;

  // set up ADC switching on timer 2
  // CTC
  TCCR2A = _BV(WGM21);
  // enable interrupts
  TIMSK2 = _BV(OCIE2A);
  // sys/64, OCR2A of 125 yields ~1000 triggers/sec
  // At the ADC clock of 125kHz, that allows 125 ADC clocks per trigger
  OCR2A = 125;
  TCCR2B = _BV(CS22);

  // this only works b/c the steady/pattern switch is also
  // the power switch (SPDT w/ off position)
  if( PIND & _BV(PIN_MODE) ) {
    // We're splitting time between the LED strings - effectively, each string
    // does one on/off cycle then spends an entire PWM period off while the
    // other string does its on/off cycle. This effectively halves the PWM
    // frequency, meaning we need really fast PWM!
    // no prescaling, 8MHz tick, 31250Hz PWM freq, 15625Hz effective PWM freq
    TCCR0B = _BV(CS00);
    // we also need to know when the next PWM cycle starts so we can swap banks
    TIMSK0 = _BV(TOIE0);

    while( 1 ) {
      OCR0A = bright_val;
      OCR0B = bright_val;
      if( flags & _BV(F_TIMER0_OVF) ) {
        flags &= ~_BV(F_TIMER0_OVF);
        flags ^= _BV(F_LED_BANK);
        set_led_bank((flags & _BV(F_LED_BANK)) >> F_LED_BANK);
      }
    }

  } else {
    // This is sticking to one string at a time (they will be switching, but
    // slow enough that flickering isn't a problem). Therefore, we can use
    // slower PWM.
    // sys/8, 1MHz tick, ~3.9kHz PWM freq
    TCCR0B = _BV(CS01);

    // set up pattern timing on timer1
    // compare match interrupt
    TIMSK1 = _BV(OCIE1A);
    // set OCR to min delay until we can read the real one
    OCR1A = DELAY_MIN;
    // CTC, sys/1024, 7812.5Hz tick
    TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);

    while( 1 ) {
      OCR0A = bright_val;
      OCR0B = bright_val;
      if( PIND & _BV(PIN_PATTERN) ) {
        // blink mode
        // sys/1024 prescaler, max delay possible 8.39 secs
        if( flags & _BV(F_ADC_DONE) ) {
          flags &= ~_BV(F_ADC_DONE);
          OCR1A = DELAY_MIN + 
            (((DELAY_MAX - DELAY_MIN) / (uint16_t)256) * (uint16_t)delay_val);
          if( (uint16_t)TCNT1 > (uint16_t)OCR1A ) {
            // we've missed the compare, fake a match and start over
            flags |= _BV(F_TIMER1_TRIG);
            TCNT1 = 0x0000;
          }
        }
        if( flags & _BV(F_TIMER1_TRIG) ) {
          flags &= ~_BV(F_TIMER1_TRIG);
          flags ^= _BV(F_LED_BANK);
          set_led_bank((flags & _BV(F_LED_BANK)) >> F_LED_BANK);
        }

      } else {
        // fade mode
      }
    }
  }

  // we should never get here!
  return 0;
}
