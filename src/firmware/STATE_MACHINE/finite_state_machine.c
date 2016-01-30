// Finite State Machine Definition
#include "finite_state_machine.h"


/*
NAME:   InitializeStateMachineStructure
INPUT   pointer to a SYSTEM to allocate and initialize
RETURN: an allocated and initialized machine on success
        NULL on error
*/

SYSTEM *InitializeStateMachineStructure (SYSTEM *machine)
{
    machine = NULL;
    machine = (SYSTEM*)malloc(sizeof(SYSTEM));
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
CHECK_t ProcessReceivedCommand (void)
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
            CLOSE_VALVE();
            SystemStructure->state = IDLE;
            break;    

        case GET_MOISTURE_READING_COMMAND:
            sprintf(packet, "/%d/", (int)(SystemStructure->moisture_reading));
            SEND_PACKET(packet);
            break;

        case GET_TEMP_READING_COMMAND:
            sprintf(packet, "/%f/", SystemStructure->temperature);
            SEND_PACKET(packet);
            break; 

        case CHANGE_MOISTURE_SETPOINT:
            if (rxBuffer[1] <= 0) sprintf("/%d/", NEGATIVE_MOISTURE_SETPOINT_ERROR);
            else if (rxBuffer[1] + SystemStructure->moisture_offset <= MAX_MOISTURE_THRESHOLD) SystemStructure->moisture_setpoint = rxBuffer[1];
            break;

        case CHANGE_MOISTURE_OFFSET:
            if (rxBuffer[1] <= 0) sprintf("/%d/", NEGATIVE_MOISTURE_OFFSET_ERROR);
            else if ((rxBuffer[1] + SystemStructure->moisture_setpoint) <= MAX_MOISTURE_THRESHOLD) SystemStructure->moisture_setpoint = rxBuffer[1];
            break;
        default:
        sprintf("/%d/", UNRECOGNIZED_COMMAND_ERROR);
        break;
    }
    CLEAR_RX_BUFFER();
}

/*
NAME:   ChangeState
RETURN: SUCCESS on successful state change
        ERROR on failure to change state
*/
CHECK_t ChangeState (void)
{
    if ()
}

/* Actuates valve open */
void OPEN_VALVE (void)
{
    VALVE_SWITCH_OPEN;  // PB5
}

/* Actuates valve closed */
void CLOSE_VALVE (void)
{
    VALVE_SWITCH_CLOSE; // PB5
}

#endif