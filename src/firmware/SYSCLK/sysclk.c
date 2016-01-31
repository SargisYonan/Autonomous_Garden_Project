#include <avr/io.h>
#include <avr/interrupt.h>
#include "../STATE_MACHINE/finite_state_machine.h"
#include "sysclk.h"

int count;

void idle_hours(int hours) {

  cli();

  SystemStatus->state = IDLE;

  count = (hours * 60 * 60 * 16000); // 16000 is because of clock speed

  TCCR1A = 0;    // set entire TCCR1A register to 0

  TCCR1B = 0;    // set entire TCCR1B register to 0

  TCCR1B |= (1 << CS10) | (1 << CS12); // clk/1024 prescaler

  sei();

}

void idle_mins(int mins) {

  cli();

  SystemStatus->state = IDLE;

  count = mins;//(mins * 60 * 16000); // 16000 is because of clock speed
  TCCR1A = 0;    // set entire TCCR1A register to 0

  TCCR1B = 0;    // set entire TCCR1B register to 0

  TCCR1B |= (1 << CS10) | (1 << CS12); // clk/1024 prescaler

  sei();

}

ISR(TIMER1_OVF_vect) {

  TCCR1B |= (1 << CS10) | (1 << CS12); // clk/1024 prescaler
  uart0_puts("TEIUYSKJS>FKUHSÇÒH");
  if (--count <= 0) {
    SystemStatus->state = VALVE_CLOSED;
    TCCR1B = 0;
  }

}
