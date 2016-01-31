// Finite State Machine Definition
#include "finite_state_machine.h"
#include "../SYSCLK/sysclk.h"

/*
NAME:   InitializeStateMachineStructure
INPUT   pointer to a SYSTEM to allocate and initialize
RETURN: an allocated and initialized machine on success
        NULL on error
*/

SYSTEM *InitializeStateMachineStructure (void)
{
    SYSTEM *machine = NULL;
    machine = (SYSTEM*)malloc(sizeof(SYSTEM));
    if (machine)
    {
        machine->state = IDLE;
        machine->moisture_reading = 0;
        machine->moisture_setpoint = DEFAULT_MOISTURE_SETPOINT;
        machine->moisture_offset = DEFAULT_MOISTURE_OFFSET;
        machine->temperature = 0.0;
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
    memset(rxBuffer, '\0', MAX_RX_LENGTH);
    memset(packet, '\0', MAX_PACKET_SIZE);

    for (int i = 0; i < MAX_RX_LENGTH; i++)
    {
        if (RX_BUFFER_EMPTY) _delay_ms(RX_TIMEOUT);

        rxBuffer[i] = GRAB_RX_BYTE;
        if (rxBuffer[i] == RX_DELIMITER) 
        {
            rxBuffer[i] = '\0';
            break;
        }
    }
    _delay_ms(RX_TIMEOUT);
    switch (rxBuffer[0])
    {
        case HALT_SYSTEM:
            idle_mins(rxBuffer[1]);
            sprintf(packet, "\n%d", rxBuffer[1]);
            SEND_PACKET(packet);

            break;
        case GET_STATUS_COMMAND:
            sprintf(packet, "/%d/%d/%d/%f/%d/", 
                (int)SystemStatus->moisture_reading, 
                (int)SystemStatus->moisture_offset, 
                (int)SystemStatus->moisture_setpoint, 
                SystemStatus->temperature, 
                (int)SystemStatus->state);
            SEND_PACKET(packet);
            break;

        case ENABLE_SYSTEM_COMMAND:
            SystemStatus->state = VALVE_CLOSED;
            sprintf(packet, "/%d/", ENABLED_SYSTEM_SUCCESSFULLY);
            SEND_PACKET(packet);
            break;  

        case DISABLE_SYSTEM_COMMAND:
            CLOSE_VALVE();
            SystemStatus->state = IDLE;
            sprintf(packet, "/%d/", DISABLED_SYSTEM_SUCCESSFULLY);
            SEND_PACKET(packet);
            break;    

        case GET_MOISTURE_READING_COMMAND:
            sprintf(packet, "/%d/", (int)(SystemStatus->moisture_reading));
            SEND_PACKET(packet);
            break;

        case GET_TEMP_READING_COMMAND:
            sprintf(packet, "/%f/", SystemStatus->temperature);
            SEND_PACKET(packet);
            break; 

        case CHANGE_MOISTURE_SETPOINT:
            if (rxBuffer[1] <= 0) sprintf(packet, "/%d/", NEGATIVE_MOISTURE_SETPOINT_ERROR);
            else if ((rxBuffer[1] + SystemStatus->moisture_offset) <= MAX_MOISTURE_THRESHOLD)
            {
                SystemStatus->moisture_setpoint = rxBuffer[1];
                sprintf(packet, "/%d/", CHANGED_MOISTURE_SETPOINT_SUCCESSFULLY);
                SEND_PACKET(packet);
            }
            break;

        case CHANGE_MOISTURE_OFFSET:
            if (rxBuffer[1] <= 0) sprintf(packet, "/%d/", NEGATIVE_MOISTURE_OFFSET_ERROR);
            else if ((rxBuffer[1] + SystemStatus->moisture_setpoint) <= MAX_MOISTURE_THRESHOLD)
            {
                SystemStatus->moisture_setpoint = rxBuffer[1];
                sprintf(packet, "/%d/", CHANGED_MOISTURE_OFFSET_SUCCESSFULLY);
                SEND_PACKET(packet);            
            }
            break;

        default:
            sprintf(packet, "/%d/", UNRECOGNIZED_COMMAND_ERROR);
            SEND_PACKET(packet);
            break;
    }
    CLEAR_RX_BUFFER();
    return SUCCESS;
}

/*
NAME:   ChangeState
RETURN: SUCCESS on successful state change
        ERROR on failure to change state
*/
CHECK_t ChangeState (void)
{
    if (SystemStatus->state == IDLE) return SUCCESS;
    if (MOISTURE_READING 
        >= (MOISTURE_SETPOINT + MOISTURE_OFFSET)
        && (CURRENT_STATE != VALVE_CLOSED))
    {
        CURRENT_STATE = VALVE_CLOSED;
        CLOSE_VALVE();
    }
    else if (MOISTURE_READING 
        <= (MOISTURE_SETPOINT - MOISTURE_OFFSET)
        && (CURRENT_STATE != VALVE_OPENED))
    {
        CURRENT_STATE = VALVE_OPENED;
        OPEN_VALVE();
    }

    return SUCCESS;
}

/* Actuates valve open */
void OPEN_VALVE (void)
{
    PORTB = 0x80;//\if (VALVE_STATUS_CLOSED) VALVE_SWITCH_OPEN;  // PB5
}

/* Actuates valve closed */
void CLOSE_VALVE (void)
{
    PORTB = 0x00;//if (VALVE_STATUS_OPEN) VALVE_SWITCH_CLOSE; // PB5
}