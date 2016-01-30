// Finite State Machine Definition

#ifndef _FINITE_STATE_MACHINE_
#define _FINITE_STATE_MACHINE_


#include "../common.h"
#include "../COMMANDS/commands_list"
#include "../COMMANDS/command_library.h"


#define MSTR_t uint16_t
#define NUMBER_OF_STATES 3


typedef enum FSM_STATES 
{
    IDLE = 0,
    VALVE_CLOSED,
    VALVE_OPENED
} States;


typedef struct
{
    State state;
    MSTR_t moisture_reading;
    MSTR_t moisture_offset;
    MSTR_t moisture_setpoint; 
    float temperature;
} FSM;


typedef struct
{
    void(*OutputFunction)(void);   // OUTPUT FUNCTION
} STATE_FUNCTIONS;
STATE_FUNCTIONS *Output;


/*
NAME:   InitializeStateMachineStructure
INPUT   pointer to an FSM to allocate and initialize
RETURN: an allocated and initialized machine on success
        NULL on error
*/

FSM *InitializeStateMachineStructure (FSM *machine);

/*
NAME:   ProcessReceivedCommand
RETURN: SUCCESS on successful manipulation
        ERROR on failure to change state
*/
CHECK_t ProcessReceivedCommand (void);


/*
NAME:   ChangeState
RETURN: SUCCESS on successful state change
        ERROR on failure to change state
*/
CHECK_t ChangeState (void);


/* Actuates valve open */
void OPEN_VALVE (void);

/* Actuates valve closed */
void CLOSE_VALVE (void);

#endif