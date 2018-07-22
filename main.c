#include "lights.h"
#include "simple_math.h"
#include "state_machine.h"

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

state_machine_t state_machine;
lights_ctl_t lights;

bool tick_flag;
void tick_callback() { tick_flag = true; }

int main(void) {
    tick_flag = false;
    /* Stop the watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Set MCLK to run from the DCO at 16 MHz */
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    /* fSMCLK = fDCO/1 = 16MHz */
    BCSCTL2 = DIVS_0;

    _BIC_SR(GIE);

    lights_init(&lights, tick_callback);
    state_machine_init(&state_machine);

    _BIS_SR(GIE);

    uint32_t counter;

    while (1) {
        if (tick_flag) {
            tick_flag = false;
            state_machine_tick(&state_machine, &lights);
        };  // Do nothing
    }

    return 0;
}

__attribute__((__interrupt__(PORT2_VECTOR)))
__attribute__((__interrupt__(PORT1_VECTOR)))
__attribute__((__interrupt__(WDT_VECTOR)))
__attribute__((__interrupt__(NMI_VECTOR))) static void
ISR_trap(void) {
    // the following will cause an access violation which results in a PUC reset
    WDTCTL = 0;
}
