#include "Arduino.h"
#include "wiring_private.h"
#include "adc.h"

#if F_CPU == 16000000L
#define ADC_CYCLES 20
#elif F_CPU == 8000000L
#define ADC_CYCLES 10
#else
#error Invalid F_CPU!
#endif

static uint8_t chan = 0;
static uint8_t idx = 0;
static uint8_t cnt = ADC_CYCLES;
static uint8_t mux = DEFAULT << 6;
static volatile uint16_t values[ADC_CHAN_NUM];
static uint16_t value = 0;
static const uint8_t
        FLAG_ADC = 0x01,
        FLAG_START = 0x02,
        FLAG_WORK = 0x04,
        FLAG_DONE = 0x08;
static volatile uint8_t flags = 0;

#if defined USR_TM0
void adc_setup(uint8_t ref, bool ext_timer)
#else
void adc_setup(uint8_t ref)
#endif
{
    mux = ref << 6;
    uint8_t low, high;
    for (int i = 0; i < ADC_CHAN_NUM; ++i) {
        ADMUX = mux | (i < 9 ? i : i + 5);
        delay(1);
        sbi(ADCSRA, ADSC);
        while (bit_is_set(ADCSRA, ADSC))
            ;
        low  = ADCL;
        high = ADCH;
        values[i] = (high << 8) | low;
    }
    ADMUX = mux;
#if defined USR_TM0
    if (!ext_timer) {
        uint8_t oldSREG = SREG;
        cli();
        user_timer0_func = &adc_update;
        SREG = oldSREG;
    }
#endif
}

uint16_t adc_get(uint8_t chan)
{
    uint16_t val;
    uint8_t oldSREG = SREG;
    cli();
    val = values[chan];
    SREG = oldSREG;
    return val;
}

void adc_update()
{
    if (flags & FLAG_ADC) {
        uint8_t low = ADCL;
        uint8_t high = ADCH;
        value += (high << 8) | low;
    }
    if (!--cnt) {
        if (flags & FLAG_ADC) {
            values[idx] = value / ADC_CYCLES;
            value = 0;
            if (++idx < 11)
                chan = idx < 9 ? idx : idx + 5;
            else {
                idx = chan = 0;
                if (flags & FLAG_WORK) {
                    flags |= FLAG_DONE;
                    flags &= ~FLAG_WORK;
                }
                if (flags & FLAG_START) {
                    flags |= FLAG_WORK;
                    flags &= ~FLAG_START;
                }
            }
            ADMUX = mux | chan;
            flags &= ~FLAG_ADC;
        } else
            flags |= FLAG_ADC;
        cnt = ADC_CYCLES;
    }
    sbi(ADCSRA, ADSC);
}

void adc_start()
{
    uint8_t oldSREG = SREG;
    cli();
    flags &= ~(FLAG_WORK | FLAG_DONE);
    flags |= FLAG_START;
    SREG = oldSREG;
}

uint8_t adc_ready()
{
    return flags & FLAG_DONE;
}
