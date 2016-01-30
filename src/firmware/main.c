// main.c


#include "common.h"
#include "adc.h"


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
