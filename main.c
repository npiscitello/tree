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

// pattern switch ISR
ISR(INT0_vect) {
  flags |= _BV(F_PATTERN_CHANGED);
}

int main(void) {
  // set up inputs/outputs
  // main LEDs: output, OC0A/OC0B (PD5/PD6)
  // pattern switch (blink/fade): input, pullup, INT0 (PD2)
  // mode switch (steady/pattern): input, no pullup, INT1 (PD3)
  DDRD = _BV(DD_LEDA) | _BV(DD_LEDB);
  PORTD = _BV(PORT_PATTERN);

  // a nice low freq is fine - let's try 250k
  //CLKPR = _BV(CLKPCE);
  //CLKPR = _BV(CLKPS2) | _BV(CLKPS0);

  // turn on what we need, turn off what we don't
  //  - Timer 0: output PWM
  //  - Timer 1: blink/fade delay timing
  //  - ADC: read delay time potentiometer
  PRR = ~_BV(PRTIM0) & ~_BV(PRTIM1) & ~_BV(PRADC);

// we need interrupts for the pattern switch
  EICRA = _BV(ISC00);
  EIMSK = _BV(INT0);
  asm("sei");

  // this only works b/c the steady/pattern switch is also
  // the power switch (SPDT w/ off position)
  if( PIND & _BV(PIN_MODE) ) {
    // steady on mode
    // we need to poll the switch the first time through
    flags |= _BV(F_PATTERN_CHANGED);
    while( 1 ) {
      if( flags & _BV(F_PATTERN_CHANGED) ) {
        flags &= ~_BV(F_PATTERN_CHANGED);
        // LED outputs CANNOT be on simultaneously
        PORTD &= ~_BV(PORT_LEDA) & ~_BV(PORT_LEDB);
        if( PIND & _BV(PIN_PATTERN) ) {
          PORTD |= _BV(PORT_LEDA);
        } else {
          PORTD |= _BV(PORT_LEDB);
        }
      }
      // go to power-down mode (see datasheet para 10.5)
    }

  } else {
    while( 1 ) {
      // read ADC (to get function speed)
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
