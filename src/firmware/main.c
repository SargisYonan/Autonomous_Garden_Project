// main.c


#include "common.h"
#include "ADC/adc.h"
#include "SYSCLK/sysclk.h"
#include "SENSOR_DRIVER/sensors.h"
#include "UART/uart.h"
#include "STATE_MACHINE/finite_state_machine.h"

CHECK_t SysInit (void);

STATE_FUNCTIONS Output[] = {{&CLOSE_VALVE},{&CLOSE_VALVE},{&OPEN_VALVE}};



int main (void)
{
    if (!SysInit()) main();
    while (TRUE)
    {
        if (uart0_available() >= 1) ProcessReceivedCommand();
        GrabMoisture();
        GrabTemperature();
        ChangeState();
    }
}

CHECK_t SysInit (void)
{
  sei();      // initialize interrupt service routines
  InitADC();  // initialize analog to digital converter  
  uart0_init(UART_BAUD_SELECT(BAUDRATE,F_CPU)); // initialize USART serial communication channels
  VALVE_RELAY_DDR |= VALVE_RELAY_DDR_SETTING;   // initialize valve switching port to an output
  SystemStatus = InitializeStateMachineStructure();
  return SUCCESS;
}
