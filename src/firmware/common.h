#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART/uart.h"


#define CHECK_t uint8_t
#define SUCCESS 1
#define ERROR   0



#define MOISTURE_SENSOR_ADC_CHANNEL 0


/* communications */
#define BAUDRATE 19200
#define MAX_RX_LENGTH 3
#define RX_BUFFER_EMPTY uart0_available() < 1
#define RX_TIMEOUT 5
#define GRAB_RX_BYTE uart0_getc()
#define CLEAR_RX_BUFFER uart0_flush

#define MAX_PACKET_SIZE 64
#define SEND_PACKET uart0_puts

#define PORT_ON(PORT,PIN) port |= (1 << pin)
#define PORT_OFF(PORT,PIN) port &= ~(1 << pin)

#define VALVE_RELAY_DDR DDRB
#define VALVE_RELAY_PORT PORTB
#define VALVE_RELAY_DDR_SETTING 0x80
#define VALVE_RELAY_PIN 5
#define VALVE_SWITCH_CLOSE PORT_OFF(VALVE_RELAY_PORT, VALVE_RELAY_PIN)
#define VALVE_SWITCH_OPEN PORT_ON(VALVE_RELAY_PORT, VALVE_RELAY_PIN)

#endif //_COMMON_H_
