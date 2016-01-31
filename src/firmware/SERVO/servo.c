
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "servo.h"

void InitializeServoPin (void)
{
    SERVO_DDR |= SERVO_ON_SETTING;
    SERVO_PORT &= 0;
}

void BULLSHIT(void) 
{
  cli();
  TCCR2A = 0;    // set entire TCCR1A register to 0
  TCCR2B = 0;    // set entire TCCR1B register to 0
  TCCR2B |= (1 << CS12); // clk/1024 prescaler
  TIMSK2 |= (1 << TOIE0);
  TCNT2=0x0BDC; // this is the timer
  sei();
}

ISR(TIMER2_OVF_vect) 
{
    SERVO_PORT ^= SERVO_ON_SETTING;
}
