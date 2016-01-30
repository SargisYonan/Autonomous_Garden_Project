// main.c


#define TRUE 1
#define BAUDRATE 19200



#define PORT_ON(port,pin) port |= (1<<pin)
#define PORT_OFF(port,pin) port &= ~(1<<pin)

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART/uart.h"
uint16_t ReadADC(uint8_t ch);
void InitADC(void);




bool SysInit (void);


int main (void)
{
    char adcstr[20];
    uint16_t moisture = 0;
    if (!SysInit()) main();

    while (TRUE)
    {
        moisture = 800 - (ReadADC(0) - 223);
        //moisture = ((1023 - moisture) * 100) / 1023;
        sprintf(adcstr, "%d\n", moisture);
        uart0_puts(adcstr);
        memset(adcstr, '\0', 20);
    }
}

bool SysInit (void)
{
    sei();  
    InitADC();  
    uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU));
    return true;
}

void InitADC(void)
{
ADMUX = (1 << REFS0);                         // For Aref=AVcc;
ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Rrescalar div factor =128
}

uint16_t ReadADC(uint8_t ch)
{
   //Select ADC Channel ch must be 0-7
   //ch=ch&0b00000111;
   ADMUX |= ch;

   //Start Single conversion
   ADCSRA |= (1 << ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1 << ADIF)));

   //Clear ADIF by writing one to it
   //Note you may be wondering why we have write one to clear it
   //This is standard way of clearing bits in io as said in datasheets.
   //The code writes '1' but it result in setting bit to '0' !!!

   ADCSRA |= (1 << ADIF);

   return(ADC);
}