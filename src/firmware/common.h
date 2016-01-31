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
#define TRUE    1
#define FALSE   0


#define MOISTURE_SENSOR_ADC_CHANNEL 0

#define DEFAULT_MOISTURE_SETPOINT 800
#define DEFAULT_MOISTURE_OFFSET 25
#define MAX_MOISTURE_THRESHOLD 1023

/* communications */
#define BAUDRATE 19200
#define MAX_RX_LENGTH 3
#define RX_BUFFER_EMPTY uart0_available() < 1
#define RX_TIMEOUT 5
#define RX_DELIMITER 0x2D
#define GRAB_RX_BYTE uart0_getc()
#define CLEAR_RX_BUFFER uart0_flush

#define MAX_PACKET_SIZE 128
#define SEND_PACKET uart0_puts

#define PORT_ON(PORT,PIN) PORT |= PIN
#define PORT_OFF(PORT,PIN) PORT &= PIN

#define VALVE_RELAY_DDR DDRB
#define VALVE_RELAY_PORT PORTB
#define VALVE_RELAY_PIN PINB
#define VALVE_ON_SETTING 0x80
#define VALVE_OFF_SETTING 0x00

#define VALVE_STATUS_OPEN VALVE_RELAY_PIN & VALVE_ON_SETTING
#define VALVE_STATUS_CLOSED VALVE_RELAY_PIN & VALVE_OFF_SETTING
#define VALVE_RELAY_DDR_SETTING 0x80
#define VALVE_SWITCH_CLOSE PORT_OFF(VALVE_RELAY_PORT, VALVE_OFF_SETTING)
#define VALVE_SWITCH_OPEN PORT_ON(VALVE_RELAY_PORT, VALVE_ON_SETTING)


#define MOISTURE_SETPOINT SystemStatus->moisture_setpoint
#define MOISTURE_READING SystemStatus->moisture_reading
#define MOISTURE_OFFSET SystemStatus->moisture_offset
#define CURRENT_STATE SystemStatus->state
#endif //_COMMON_H_
