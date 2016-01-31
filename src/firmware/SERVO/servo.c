#include <stdlib.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "servo.h"
#include <stdbool.h>
bool SERVO_FLAG;
volatile int SERVO_COUNT;
volatile int SERVO_COUNT_TWO;
volatile int SERVO_TIME;
volatile int ISR_COUNT;

void InitializeServoPin (void)
{
  SERVO_FLAG = false;
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
  ISR_COUNT = 0;
  SERVO_TIME = 0;
  SERVO_COUNT = 0;
  if (state == SERVO_OPEN) SERVO_COUNT_TWO = 3;
  else SERVO_COUNT_TWO = 4;
  TCCR3A = 0;    // set entire TCCR1A register to 0
  TCCR3B = 0;    // set entire TCCR1B register to 0
  TCCR3B |= (1 << CS11); // clk/8 prescaler
  OCR3A = 999;
  TCCR3B |= (1 << WGM12); // compare mode stuff
  TIMSK3 |= (1 << OCIE1A);
  //TCNT3 = 1000; // this should effectively make a .5 ms ISR
  SERVO_PORT &= 0x00;
  SERVO_PORT |= SERVO_ON_SETTING;
  sei();

  /*
     cli();
     freq  = 0;
     TCCR2A = 0;    // set entire TCCR2A register to 0
     TCCR2B = 0;    // set entire TCCR2B register to 0
     TCCR2B |= (1 << CS10); // no prescaler
     TIMSK2 |= (1 << TOIE0);
     TCCR3A = 0;    // set entire TCCR0A register to 0
     TCCR3B = 0;    // set entire TCCR0B register to 0
     TCCR3B |= (1 << CS10); // no prescaler
     TIMSK3 |= (1 << TOIE0);
     servo_state->duty_count = (servo_state->state == SERVO_OPEN ? 10 : 13);
     servo_state->state = state;
     servo_state->duty_cycle_state = SERVO_DUTY_ON;
     TCNT2 = ((servo_state->state == SERVO_OPEN) ? 0x0612 : 0x048E); // this is the timer
     TCNT3 = 0xF2E1;
     sei();
   */
}

ISR(TIMER3_COMPA_vect) // now in compare mode
{
  //if (ISR_COUNT == 1000) {
    //ISR_COUNT = 0;
    if (SERVO_TIME >= 0) SERVO_TIME++;
    if (SERVO_COUNT == 40)
    {
      SERVO_PORT |= SERVO_ON_SETTING;
      SERVO_COUNT = 0;
      SERVO_TIME = 0;
    }
    else SERVO_COUNT++;


    if (SERVO_COUNT_TWO == SERVO_TIME) 
    {
      SERVO_PORT &= 0x00;
      SERVO_TIME = -1; 
    }

  //} else {
  //  ISR_COUNT++;
 // }
  //OCR3A = 999;
}
