#include <msp430.h>

#include "lights.h"
#include "util/numeric_utils.h"

#define SMCLK_FREQ (16000000 / 4)
#define TA_FREQ (SMCLK_FREQ / 2)
#define TA_PERIOD (65536)
#define LIGHTS_SCALE_VAL NU_DIV_ROUND_UP(TA_PERIOD, 256)

static lights_ctl_t* lights_ctl;

void lights_init(lights_ctl_t* control)
{
    lights_ctl = control;

    TA1CTL = TACLR;
    TA1CTL = TASSEL_2 | ID_1 | TAIE;

    TA1CCR0 = TA_PERIOD - 1;
    TA1CCR1 = TA1CCR2 = 0;

    TA1CCTL1 = TA1CCTL2 = OUTMOD_7;

    /* Configure P1.2 as TA1.1 */
    P2DIR |= 0x14;
    P2SEL |= 0x14;
    P2SEL2 &= ~0x14;

    TA1CTL |= MC_1;
}

__attribute__((__interrupt__(TIMER1_A1_VECTOR)))
void ISR_timer1_a1()
{
    TA1CTL &= ~TAIFG;

    TA1CCR1 = lights_ctl->level[0] * LIGHTS_SCALE_VAL;
    TA1CCR2 = lights_ctl->level[1] * LIGHTS_SCALE_VAL;
}
