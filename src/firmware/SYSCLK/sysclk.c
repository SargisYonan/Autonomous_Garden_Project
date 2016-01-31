#include <avr/io.h>
#include <avr/interrupt.h>
#include "../STATE_MACHINE/finite_state_machine.h"
#include "sysclk.h"

float count;
States lastState;
char finishedPacket[] = "0x81";

void idle_hours(float hours) 
{
  cli();
  lastState = SystemStatus->state;
  SystemStatus->state = IDLE;

  count = (hours * 3600);
  TCCR1A = 0;    // set entire TCCR1A register to 0
  TCCR1B = 0;    // set entire TCCR1B register to 0
  TCCR1B |=(1 << CS12); // clk/1024 prescaler
  TIMSK1 |= (1 << TOIE1);
  TCNT1=0x0BDC; // this is the timer
  sei();
}

void idle_mins(float mins) 
{
  cli();
  lastState = SystemStatus->state;
  SystemStatus->state = IDLE;
  count = (mins * 60); // 2 is because default ISR hits every .5 sec
  TCCR1A = 0;    // set entire TCCR1A register to 0
  TCCR1B = 0;    // set entire TCCR1B register to 0
  TCCR1B |= (1 << CS12); // clk/1024 prescaler
  TIMSK1 |= (1 << TOIE1);
  TCNT1=0x0BDC; // this is the timer
  sei();
}

ISR(TIMER1_OVF_vect) 
{
  if (--count <= 0) 
  {
    SEND_PACKET(finishedPacket);
    SystemStatus->state = lastState;
    TCCR1B = 0;
  } else {
    TCNT1=0x0BDC;
  }

}
