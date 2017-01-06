#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>

#include "i2c_memdev.h"
#include "lights.h"
#include "servo.h"
#include "battery.h"

typedef struct
{
    servo_ctl_t servos;
    lights_ctl_t lights;
} control_t;

typedef struct
{
    battery_t battery;
} monitor_t;

typedef struct
{
    control_t control;
    monitor_t monitor;
} memmap_t;

static memmap_t memmap;

int main(void)
{
    /* Stop the watchdog */
    WDTCTL = WDTPW + WDTHOLD;

    /* Set MCLK to run from the DCO at 16 MHz */
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    /* fSMCLK = fDCO/4 = 4MHz */
    BCSCTL2 = DIVS_2;

    _BIC_SR(GIE);

    i2c_init_mem(0x42);
    i2c_init_readmem((uint8_t*)&memmap, sizeof(memmap));
    i2c_init_writemem((uint8_t*)&memmap.control, sizeof(memmap.control));
    servo_init(&memmap.control.servos);
    lights_init(&memmap.control.lights);
    battery_init(&memmap.monitor.battery);

    _BIS_SR(GIE);

    while (1)
    {
        ;   // Do nothing
    }

    return 0;
}

//__attribute__((__interrupt__(TIMER0_A0_VECTOR)))
__attribute__((__interrupt__(PORT2_VECTOR)))
__attribute__((__interrupt__(PORT1_VECTOR)))
__attribute__((__interrupt__(WDT_VECTOR)))
__attribute__((__interrupt__(NMI_VECTOR)))
static void ISR_trap(void) {
    while(1)
        ;

    // the following will cause an access violation which results in a PUC reset
    //WDTCTL = 0;
}
