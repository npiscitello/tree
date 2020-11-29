#include "avr/io.h"
#include "avr/interrupt.h"

// all tick calcs assume 8MHz clock

// min/max number of ticks for timer1
#define BLINK_DELAY_MIN   (7812.5 * 0.075)        // 0.075 sec/blink
#define BLINK_DELAY_MAX   (7812.5 * 2)            // 2 sec/blink
#define FADE_DELAY_MIN    (31250 * (0.5 / 256))   // 0.5 sec/fade
#define FADE_DELAY_MAX    (31250 * (4 / 256))     // 4 sec/fade

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
#define F_TIMER1_TRIG     6
#define F_ADC_DONE        5
#define F_LED_BANK        4
#define F_PATTERN_BIT     3

volatile uint8_t bright_val = 0;
volatile uint8_t delay_val = 0;
volatile uint8_t* adc_reg = &bright_val;

// INT0/INT1 ISRs - mode switch changed
ISR(INT0_vect) {
  flags |= _BV(F_MODE_CHANGED);
}
ISR(INT1_vect, ISR_ALIASOF(INT0_vect));

// timer 0 ISR - swap LED banks
ISR(TIMER0_OVF_vect) {
  // We're splitting time between the LED strings - each string does one
  // on/off cycle then spends an entire PWM period off while the other
  // string does its cycle. This effectively halves the PWM freq.
  if( flags & _BV(F_LED_BANK) ) {
    TCCR0A = (TCCR0A & 0x0F) | _BV(COM0B1);
  } else {
    TCCR0A = (TCCR0A & 0x0F) | _BV(COM0A1);
  }
  flags ^= _BV(F_LED_BANK);
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

int main(void) {
  // set up inputs/outputs
  // main LEDs: output, OC0A/OC0B (PD5/PD6)
  // fade trigger: input, pullup, INT0 (PD2)
  // blink trigger: input, pullup, INT1 (PD3)
  DDRD = _BV(DD_LEDA) | _BV(DD_LEDB);
  PORTD = _BV(PORT_FADE) | _BV(PORT_BLINK);

  // drop the clock to 2MHz to support a better PWM freq
  // test compatibility on final board;
  CLKPR = _BV(CLKPCE);
  CLKPR = _BV(CLKPS1);

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
  // 8MHz System Clock
  // sys/64, 125kHz tick
  //ADCSRA = _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
  // 2MHz System Clock
  // sys/16, 125kHz tick
  ADCSRA = _BV(ADIE) | _BV(ADPS2);
  ADCSRA |= _BV(ADEN);

  // set up PWM on timer 0
  // fast PWM, 256 bit resolution
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  // 8MHz System Clock
  // sys/64, 125kHz clock, ~488Hz PWM freq, ~244 effective PWM freq
  // (see ISR, PWM freq is halved because we split time between banks)
  //TCCR0B = _BV(CS01) | _BV(CS00);
  // 2MHz System Clock
  // sys/8, 250kHz clock, ~976Hz PWM freq, ~488 effective PWM freq
  // (see ISR, PWM freq is halved because we split time between banks)
  TCCR0B = _BV(CS01);               // sys/8
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
  // 8MHz System Clock
  // sys/64, OCR2A of 63 yields ~2000 triggers/sec
  // At the ADC clock of 125kHz, that allows 63 ADC clocks per trigger
  //OCR2A = 63;
  //TCCR2B = _BV(CS22);
  // 2MHz System Clock
  // sys/32, 62.5kHz tick, OCR2A of 31 yields ~2016 triggers/sec
  // At the ADC clock of 125kHz, that allows 62 ADC clocks per trigger
  OCR2A = 31;
  TCCR2B = _BV(CS21) | _BV(CS20);

  while( 1 ) {
// fade setup //
    if( !(PIND & _BV(PIN_FADE)) ) {
      // 2MHz System Clock
      // timer 1 resets on every brightness level change (256 per fade), 
      // so we run it fast
      // CTC, sys/64, 31.25kHz tick, max delay possible 0.5 sec (537 sec fade)
      TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
      // turn on right away
      flags |= _BV(F_TIMER1_TRIG);
      // store fade state
      uint8_t fade_state = 0;

// fade loop //
      while(1) {
        bright_val = 255;
        if( flags & _BV(F_TIMER1_TRIG) ) {
          flags &= ~_BV(F_TIMER1_TRIG);
          // constant rate fade - if peak brightness is reduced, fade will take
          // less time to complete
          // calculate bank brightnesses based on fade state and direction
          if( flags & _BV(F_PATTERN_BIT) ) {
            OCR0A = bright_val - fade_state;
            OCR0B = fade_state;
          } else {
            OCR0A = fade_state;
            OCR0B = bright_val - fade_state;
          }
          // increment fade state until it reaches full brightness, then reverse
          if( fade_state++ >= bright_val ) {
            flags ^= _BV(F_PATTERN_BIT);
            fade_state = 0;
          }
        }
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }

// blink setup //
    } else if( !(PIND & _BV(PIN_BLINK)) ) {
      // 8MHz System Clock
      // timer 1 resets when the LED bank should switch, so we run it slow
      // CTC, sys/1024, 7812.5Hz tick, max delay possible 8.39 secs
      //TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
      // 2MHz System Clock
      // timer 1 resets when the LED bank should switch, so we run it very slow
      // CTC, sys/256, 7812.5Hz tick, max delay possible 8.39 secs
      TCCR1B = _BV(WGM12) | _BV(CS12);
      // turn on right away
      flags |= _BV(F_TIMER1_TRIG);
      // aliases to the reg currently doing its blink duty
      volatile uint8_t* on_reg = &OCR0A;

// blink loop //
      while(1) {
        if( flags & _BV(F_ADC_DONE) ) {
          flags &= ~_BV(F_ADC_DONE);
          *on_reg = bright_val;
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
          flags ^= _BV(F_PATTERN_BIT);
          if( flags & _BV(F_PATTERN_BIT) ) {
            OCR0A = 0x00;
            on_reg = &OCR0B;
          } else {
            on_reg = &OCR0A;
            OCR0B = 0x00;
          }
        }
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }

// steady on setup //
    } else {

// steady on loop //
      while(1) {
        if( flags & _BV(F_ADC_DONE) ) {
          flags &= ~_BV(F_ADC_DONE);
          OCR0A = bright_val;
          OCR0B = bright_val;
        }
        if( flags & _BV(F_MODE_CHANGED) ) break;
      }
    }
    flags &= ~_BV(F_MODE_CHANGED);
  }

  // we should never get here!
  return 0;
}
