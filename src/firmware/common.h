#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART/uart.h"

#define SUCCESS 1
#define ERROR   0
#define BAUDRATE 19200

#define PORT_ON(PORT,PIN) port |= (1 << pin)
#define PORT_OFF(PORT,PIN) port &= ~(1 << pin)

#endif //_COMMON_H_
