#include <stdlib.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "servo.h"

int freq = 0;
    char test[101];

void InitializeServoPin (void)
{
  SERVO_DDR |= SERVO_ON_SETTING;
  SERVO_PORT &= 0;
  servo_state = malloc(sizeof(SERVO_STATE));
  servo_state->state = SERVO_CLOSED;
}

void ChangeServoState(uint8_t state)
{
  cli();
  freq  = 0;
  TCCR2A = 0;    // set entire TCCR2A register to 0
  TCCR2B = 0;    // set entire TCCR2B register to 0
  TCCR2B |= (1 << CS10); // no prescaler
  TIMSK2 |= (1 << TOIE0);
  servo_state->state = state;
  TCNT2 = ((servo_state->state == SERVO_OPEN) ? 0x0612 : 0x048E); // this is the timer
  sei();
}

ISR(TIMER2_OVF_vect) 
{
  freq++;
  if (freq > 500) {
    sprintf(test, "if valve is open, this should have taken 1 second %d\n", freq);
    uart0_puts(test);
    freq = 0;
  }
  SERVO_PORT ^= SERVO_ON_SETTING;
  TCNT2= (servo_state->state == SERVO_OPEN ? 0x0612 : 0x048E); // this is the timer
}
