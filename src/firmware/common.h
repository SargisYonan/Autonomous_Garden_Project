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

#endif //_COMMON_H_
