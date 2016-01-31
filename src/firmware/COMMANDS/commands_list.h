#ifndef _COMMANDS_LIST_H_
#define _COMMANDS_LIST_H_


/* DEFINITIONS OF RECIEVEABLE COMMANDS */

#define GET_STATUS_COMMAND				0xAA
#define ENABLE_SYSTEM_COMMAND			0xEE
#define DISABLE_SYSTEM_COMMAND			0xDD

#define GET_MOISTURE_READING_COMMAND	0xBB
#define GET_TEMP_READING_COMMAND		0xCC 

#define HALT_SYSTEM                     0x56
#define TIMER_COMPLETE                  0x81
#define CHANGE_MOISTURE_SETPOINT        0xFF
#define CHANGE_MOISTURE_OFFSET          0x11

#define UNRECOGNIZED_COMMAND_ERROR      0x44
#define NEGATIVE_MOISTURE_SETPOINT_ERROR 0x12
#define NEGATIVE_MOISTURE_OFFSET_ERROR  0x55

#define CHANGED_MOISTURE_OFFSET_SUCCESSFULLY 0x10
#define CHANGED_MOISTURE_SETPOINT_SUCCESSFULLY 0x11
#define DISABLED_SYSTEM_SUCCESSFULLY 0x12
#define ENABLED_SYSTEM_SUCCESSFULLY 0x13

#endif
