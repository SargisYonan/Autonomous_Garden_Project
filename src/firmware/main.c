// main.c


#include "common.h"
#include "ADC/adc.h"


CHECK_t SysInit (void);

SYSTEM *SystemStatus = NULL;

int main (void)
{
    if (!SysInit()) main();
    while (TRUE)
    {
        ProcessReceivedCommand();
        GrabMoisture();
        GrabTemperature();
        ChangeState();
        STATE_FUNCTIONS[SystemStatus->state].OutputFunction();
    }
}

CHECK_t SysInit (void)
{
  sei();      // initialize interrupt service routines
  InitADC();  // initialize analog to digital converter  
  uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU)); // initialize USART serial communication channels
  VALVE_RELAY_DDR |= VALVE_RELAY_DDR_SETTING;   // initialize valve switching port to an output
  return SUCCESS;
}
