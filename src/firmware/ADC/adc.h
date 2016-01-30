#ifndef ADC_H_
#define ADC_H_

/*
ADC reference:
DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards)
        or 3.3 volts (on 3.3V Arduino boards)
INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168
        or ATmega328 and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
INTERNAL1V1: a built-in 1.1V reference (Arduino Mega only)
INTERNAL2V56: a built-in 2.56V reference (Arduino Mega only)
EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used
        as the reference.
*/

#define ADC_REF_AREF EXTERNAL
#define ADC_REF_AVCC DEFAULT
#define ADC_REF_2V56 INTERNAL2V56


#define ADC_CHAN_NUM 11

#if defined USR_TM0
void adc_setup(uint8_t ref, bool ext_timer);
#else
void adc_setup(uint8_t ref);
#endif
uint16_t adc_get(uint8_t chan);
void adc_update();
void adc_start();
uint8_t adc_ready();
#endif

