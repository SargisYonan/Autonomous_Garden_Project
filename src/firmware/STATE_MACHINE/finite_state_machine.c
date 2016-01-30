// Finite State Machine Definition
#include "finite_state_machine.h"


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

FSM *InitializeStateMachineStructure (FSM *machine)
{
    machine = NULL;
    machine = (FSM*)malloc(sizeof(FSM));
    if (machine)
    {
        machine->state = IDLE;
        machine->moisture_reading = 0;
        machine->moisture_setpoint = DEFAULT_MOISTURE_SETPOINT;
        machine->moisture_offset = DEFAULT_MOISTURE_OFFSET;
        machine->temperature = 0.0;

        Output = (STATE_FUNCTIONS*)malloc(NUMBER_OF_STATES * sizeof(STATE_FUNCTIONS));
        if (!Output)
        {
            free(machine);
            return NULL;
        }
        Output = {&CLOSE_VALVE, &CLOSE_VALVE, &OPEN_VALVE};
    }
    return machine;
}

/*
NAME:   ProcessReceivedCommand
RETURN: SUCCESS on successful manipulation
        ERROR on failure to change state
*/
CHECK_t ProcessReceivedCommand (FSM *SystemStructure)
{
    uint8_t rxBuffer[MAX_RX_LENGTH];
    char packet[MAX_PACKET_SIZE];

    memset(packet, '\0', MAX_PACKET_SIZE);

    for (int i = 0; i < MAX_RECEIVE_LENGTH; i++)
    {
        if (RX_BUFFER_EMPTY) _delay_ms(RX_TIMEOUT);

        rxBuffer[i] = GRAB_RX_BYTE;
        if (rxBuffer[i] == RX_DELIMITER) 
        {
            rxByteArray[i] = '\0';
            break;
        }
    }

    if (rxBuffer[MAX_RECEIVE_LENGTH - 1] != RX_DELIMITER) 
    {
        CLEAR_RX_BUFFER();
        return;
    }

    switch (rxBuffer[0])
    {
        case GET_STATUS_COMMAND:

        sprintf(packet, "/%d/%d/%d/%f/%d/", 
            (int)(SystemStructure->moisture_reading), 
            (int)(SystemStructure->moisture_offset), 
            (int)(SystemStructure->moisture_setpoint), 
            SystemStructure->temperature, 
            (int)(SystemStructure->state);

        SEND_PACKET(packet);
        break;

        case ENABLE_SYSTEM_COMMAND:
        SystemStructure->state = VALVE_CLOSED;
        break;    
        case DISABLE_SYSTEM_COMMAND:
        break;    

        case GET_MOISTURE_READING_COMMAND:
        break;
        case GET_TEMP_READING_COMMAND:
        break; 

        case CHANGE_MOISTURE_SETPOINT:
        break;
        case CHANGE_MOISTURE_OFFSET:
        break;
        default:
        break;
    }

    CLEAR_RX_BUFFER();
}

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