//sensors.c
#include "sensors.h"

void GrabMoisture (void)
{
    SystemStatus->moisture_reading = 800 - (ReadAdc(MOISTURE_SENSOR_ADC_CHANNEL) - 223);
}