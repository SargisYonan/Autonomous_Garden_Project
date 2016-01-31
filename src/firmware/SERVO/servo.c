
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "servo.h"

void InitializeServoPin (void)
{
  SERVO_DDR |= SERVO_ON_SETTING;
  SERVO_PORT &= 0;
  servo_state = malloc(sizeof(SERVO_STATE));
  servo_state->state = SERVO_CLOSED;
}

void changeServoState(uint8_t state)
{
  cli();
  TCCR2A = 0;    // set entire TCCR2A register to 0
  TCCR2B = 0;    // set entire TCCR2B register to 0
  TCCR2B |= (1 << CS10); // no prescaler
  TIMSK2 |= (1 << TOIE0);
  TCNT2= (servo_state == SERVO_OPEN ? 0x0612 : 0x048E); // this is the timer
  sei();
}

ISR(TIMER2_OVF_vect) 
{
  SERVO_PORT ^= SERVO_ON_SETTING;
  TCNT2= (servo_state == SERVO_OPEN ? 0x0612 : 0x048E); // this is the timer
}
