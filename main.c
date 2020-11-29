#include "avr/io.h"
#include "avr/interrupt.h"

// all tick calcs assume 8MHz clock

// min/max number of ticks for timer1
#define BLINK_DELAY_MIN   (7812.5*0.075)  // 0.075 sec/reset
#define BLINK_DELAY_MAX   (7812.5*2)      // 2 sec/reset
#define FADE_DELAY_MIN    (7812.5*0.075)  // 0.75 sec/reset
#define FADE_DELAY_MAX    (7812.5*2)      // 2 sec/reset

#define PORT_LEDB     PORTD5
#define DD_LEDB       DDD5

#define PORT_LEDA     PORTD6
#define DD_LEDA       DDD6

#define PORT_FADE     PORTD2
#define DD_FADE       DDD2
#define PIN_FADE      PIND2

#define PORT_BLINK    PORTD3
#define DD_BLINK      DDD3
#define PIN_BLINK     PIND3

volatile uint8_t flags = 0x00;
#define F_MODE_CHANGED    7
#define F_TIMER0_OVF      6
#define F_TIMER1_TRIG     5
#define F_ADC_DONE        4
#define F_LED_BANK        3

volatile uint8_t bright_val = 0;
volatile uint8_t delay_val = 0;
volatile uint8_t* adc_reg = &bright_val;

// INT0/INT1 ISRs - mode switch changed
ISR(INT0_vect) {
  flags |= _BV(F_MODE_CHANGED);
}
ISR(INT1_vect, ISR_ALIASOF(INT0_vect));

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
  if( led_bank ) {
    TCCR0A = (TCCR0A & 0x0F) | _BV(COM0B1);
  } else {
    TCCR0A = (TCCR0A & 0x0F) | _BV(COM0A1);
  }
}

int main(void) {
  // set up inputs/outputs
  // main LEDs: output, OC0A/OC0B (PD5/PD6)
  // fade trigger: input, pullup, INT0 (PD2)
  // blink trigger: input, pullup, INT1 (PD3)
  DDRD = _BV(DD_LEDA) | _BV(DD_LEDB);
  PORTD = _BV(PORT_FADE) | _BV(PORT_BLINK);

  // make double sure the clock is what we expect it to be
  //CLKPR = _BV(CLKPCE);
  //CLKPR = 0x00;

  // turn on what we need, turn off what we don't
  //  - Timer 0: output PWM
  //  - Timer 1: blink/fade delay timing
  //  - Timer 2: ADC management
  //  - ADC: read delay time/brightness potentiometer
  PRR = ~_BV(PRTIM0) & ~_BV(PRTIM1) & ~_BV(PRTIM2) & ~_BV(PRADC);

  // enable interrupts for switch, timers, and ADC
  EICRA = _BV(ISC10) | _BV(ISC00);
  EIMSK = _BV(INT1) | _BV(INT0);
  asm("sei");

  // set up ADC in oneshot mode and left adjust the result
  ADMUX = _BV(ADLAR);
  // sys/64, 125kHz tick
  ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  ADCSRA |= _BV(ADEN);

  // set up PWM on timer 0
  // fast PWM, 256 bit resolution
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  // sys/64, 125kHz clock, ~488Hz PWM freq
  TCCR0B = _BV(CS01) | _BV(CS00);
  //TCCR0B = _BV(CS01);               // sys/8
  //TCCR0B = _BV(CS02);               // sys/256
  //TCCR0B = _BV(CS02) | _BV(CS00);   // sys/1024
  // interrupt on every PWM cycle
  TIMSK0 |= _BV(TOIE0);

  // set up pattern timing on timer1
  // compare match interrupt
  TIMSK1 = _BV(OCIE1A);

  // set up ADC switching on timer 2
  // CTC
  TCCR2A = _BV(WGM21);
  // enable interrupts
  TIMSK2 = _BV(OCIE2A);
  // sys/64, OCR2A of 63 yields ~2000 triggers/sec
  // At the ADC clock of 125kHz, that allows 63 ADC clocks per trigger
  OCR2A = 63;
  TCCR2B = _BV(CS22);

  while( 1 ) {
// fade setup //
    if( !(PIND & _BV(PIN_FADE)) ) {

// fade loop //
      while(1) {
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }

// blink setup //
    } else if( !(PIND & _BV(PIN_BLINK)) ) {
      // timer 1 resets when the LED bank should switch, so we run it very slow
      // CTC, sys/1024, 7812.5Hz tick, max delay possible 8.39 secs
      TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
      // turn on right away
      flags |= _BV(F_TIMER1_TRIG);

// blink loop //
      while(1) {
        if( flags & _BV(F_ADC_DONE) ) {
          flags &= ~_BV(F_ADC_DONE);
          // cannot be zero or it results in a headache-inducing strobe
          OCR0A = bright_val | 0x01;
          OCR0B = bright_val | 0x01;
          OCR1A = BLINK_DELAY_MIN + 
            (((BLINK_DELAY_MAX - BLINK_DELAY_MIN) /
              (uint16_t)256) * (uint16_t)delay_val);
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
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }

// steady on setup //
    } else {
      // We're splitting time between the LED strings - each string does one
      // on/off cycle then spends an entire PWM period off while the other
      // string does its cycle. This effectively halves the PWM freq.

// steady on loop //
      while(1) {
        if( flags & _BV(F_ADC_DONE) ) {
          flags &= ~_BV(F_ADC_DONE);
          // cannot be zero or it results in a headache-inducing strobe
          OCR0A = bright_val | 0x01;
          OCR0B = bright_val | 0x01;
        }
        if( flags & _BV(F_TIMER0_OVF) ) {
          flags &= ~_BV(F_TIMER0_OVF);
          flags ^= _BV(F_LED_BANK);
          set_led_bank((flags & _BV(F_LED_BANK)) >> F_LED_BANK);
        }
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }
    }
    flags &= ~_BV(F_MODE_CHANGED);
  }

  // we should never get here!
  return 0;
}
