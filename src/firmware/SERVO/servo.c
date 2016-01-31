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
  servo_state->duty_cycle_state = SERVO_DUTY_OFF;
  servo_state->duty_count = 13;
}

void ChangeServoState(uint8_t state)
{
  cli();
  freq  = 0;
  TCCR2A = 0;    // set entire TCCR2A register to 0
  TCCR2B = 0;    // set entire TCCR2B register to 0
  TCCR2B |= (1 << CS10); // no prescaler
  TIMSK2 |= (1 << TOIE0);
  servo_state->duty_count = (servo_state->state == SERVO_OPEN ? 10 : 13);
  servo_state->state = state;
  servo_state->duty_cycle_state = SERVO_DUTY_ON;
  TCNT2 = ((servo_state->state == SERVO_OPEN) ? 0x0612 : 0x048E); // this is the timer
  sei();
}

ISR(TIMER2_OVF_vect) 
{
  servo_state->duty_count--;
  if (servo_state->duty_cycle_state == SERVO_DUTY_ON) {
    servo_state->duty_cycle_state = SERVO_DUTY_OFF;
    SERVO_PORT ^= SERVO_ON_SETTING;
  } else if (servo_state->duty_count <= 0) {
    SERVO_PORT ^= SERVO_ON_SETTING;
    servo_state->duty_cycle_state = SERVO_DUTY_ON;
    servo_state->duty_count = (servo_state->state == SERVO_OPEN ? 10 : 13);
  }
  /*
  freq++;
  if (freq > 500) {
    sprintf(test, "if valve is open, this should have taken 1 second %d\n", freq);
    uart0_puts(test);
    freq = 0;
  }
  */
  //SERVO_PORT ^= SERVO_ON_SETTING;
  TCNT2= (servo_state->state == SERVO_OPEN ? 0x0612 : 0x048E); // this is the timer
}
