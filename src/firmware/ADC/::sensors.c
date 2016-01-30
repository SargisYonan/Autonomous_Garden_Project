//sensors.c
#include "sensors.h"

void GrabMoisture (void)
{
    SystemStatus->moisture_reading = ReadAdc(MOISTURE_SENSOR_ADC_CHANNEL);
}