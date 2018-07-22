#include <msp430.h>

#include "battery.h"

static battery_t* battery;

void battery_init(battery_t* bat) {
    battery = bat;

    /* Configure ADC10 with:
     * Vcc/Vss reference
     * 64x ADC10CLK cycles per conversion
     * 50ksps reference buffer
     * continuous conversion
     * turn on the ADC10
     */
    ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10SR | ADC10IE | MSC | ADC10ON;

    ADC10CTL1 = INCH_0 | SHS_0 | ADC10DIV_7 | ADC10SSEL_3 | CONSEQ_2;

    ADC10AE0 = 0x01;

    ADC10CTL0 |= ADC10SC | ENC;
}

__attribute__((__interrupt__(ADC10_VECTOR))) void ISR_adc10() {
    ADC10CTL0 &= ~ADC10IFG;

    *battery = ADC10MEM;
}
