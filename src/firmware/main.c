// main.c


#define TRUE 1
#define BAUDRATE 19200

#include <stdbool.h>


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART/uart.h"



bool SysInit (void);


int main (void)
{
    while (TRUE)
    {
        if (!SysInit()) main();
        
        uart0_puts("TEST\n");
        _delay_ms(100);

    }
}

bool SysInit (void)
{
    sei();    
    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    return true;
}