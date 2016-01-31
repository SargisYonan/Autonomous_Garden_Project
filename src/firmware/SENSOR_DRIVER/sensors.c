//sensors.c
#include "sensors.h"
#include "../ADC/adc.h"

void GrabMoisture (void)
{
    SystemStatus->moisture_reading = 800 - (ReadADC(MOISTURE_SENSOR_ADC_CHANNEL) - 223);
}

void GrabTemperature (void)
{
    return;
}