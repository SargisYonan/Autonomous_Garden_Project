/*
*
*   Servo/PWM/Timer related stuff
*
*/

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "servo.h"
struct servo servos[8];

int init_servos(void)
{
    //Init all servo structures

    int i;
    for(i=0;i<8;i++) {
        servos[i].duty = 3000;
        servos[i].state = 1;
    }

    //Init Port C for all outputs and set to off (0).
    //These are the first six servo signal ports.
    DDRC    = 0xFF;
    PORTC   = 0x00;

    //Init Timer 1 (for servos)
    OCR1A = servos[0].duty;
    TCCR1B = TIMER_1_8_PRESCALE;
    TIMSK1 |= TIMER_1_IR_ENABLE;

    sei();  //Enable Interrupts

    return 1;
}


// Set individual servo PWM duty cycle.
int set_servo(int servo, int value) {
    if( value > PWM_MAX || value < PWM_MIN || servo > 7 || servo < 0 ) {
        return 0;
    } else {
        servos[servo].duty = value;
    }
    return 1;
}

//Retrieve individual servo PWM duty cycle.
int get_servo(int servo) {
    if(servo >= 0 && servo < 8) 
        return servos[servo].duty;
    else
        return -1;
}


//Interrupt functions for driving the servos

//If somehow there is an unknown Interrupt, print an error
ISR(BADISR_vect) 
{ 
    printf("BAD ISR!\n\n");
}


//This is the Timer 1 Interrupt that drives the servos
// The algorithm is fairly simple and can only handle up
// to about 8 servos. More than that and a slightly more
// complicated one would be needed. What this does is step
// through each servo one at a time turning it on, waiting
// the amount of time it is set to be on, then turning it off.
// since it does each servo one at a time, if the sum of all
// the on times is greater than the PWM period, it wont work.
ISR(TIMER1_COMPA_vect)
{
    //Maintain the current servo we are on
    static int pwm_state = 0;
    
    if(pwm_state < 8) {
        PORTC ^= (128 >> pwm_state);        //Turn on next servo port
    
        if(pwm_state)
            PORTC ^= (128 >> (pwm_state-1));    //Turn off last servo port
        OCR1A = servos[pwm_state].duty;

        pwm_state++;
    } else {
        PORTC ^= 0x01;  //Turn off servo 8 port
        OCR1A = PWM_PERIOD - servos[0].duty - servos[1].duty - servos[2].duty - servos[3].duty - servos[4].duty - servos[5].duty - servos[6].duty - servos[7].duty;
        pwm_state = 0;
    }   
    
    TCNT1 = 0;
}