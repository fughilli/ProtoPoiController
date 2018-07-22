#include <msp430.h>

#include "lights.h"
#include "util/numeric_utils.h"

#define SMCLK_FREQ (16000000)
#define TA_FREQ (SMCLK_FREQ)
#define TA_PERIOD (256)
#define LIGHTS_SCALE_VAL NU_DIV_ROUND_UP(TA_PERIOD, 256)

static lights_ctl_t* lights_ctl;
static tick_callback_t tick_callback;

void lights_init(lights_ctl_t* control, tick_callback_t callback) {
    lights_ctl = control;
    tick_callback = callback;

    P1DIR |= 0x40;
    P1SEL |= 0x40;
    P1SEL2 &= ~0x40;

    P2DIR |= 0x22;
    P2SEL |= 0x22;
    P2SEL2 &= ~0x22;

    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 | ID_0 | TAIE;

    TA0CCR0 = TA_PERIOD - 1;
    TA0CCR1 = 0;

    TA0CCTL1 = OUTMOD_7;

    TA1CTL = TACLR;
    TA1CTL = TASSEL_2 | ID_1 | TAIE;

    TA1CCR0 = TA_PERIOD - 1;
    TA1CCR1 = TA1CCR2 = 0;

    TA1CCTL1 = TA1CCTL2 = OUTMOD_7;

    TA0CTL |= MC_1;
    TA1CTL |= MC_1;
}

__attribute__((__interrupt__(TIMER0_A1_VECTOR))) void ISR_timer0_a1() {
    TA0CTL &= ~TAIFG;

    TA0CCR1 = (lights_ctl->level[0]) * LIGHTS_SCALE_VAL;

    tick_callback();
}

__attribute__((__interrupt__(TIMER1_A1_VECTOR))) void ISR_timer1_a1() {
    TA1CTL &= ~TAIFG;

    TA1CCR1 = (lights_ctl->level[1]) * LIGHTS_SCALE_VAL;
    TA1CCR2 = (lights_ctl->level[2]) * LIGHTS_SCALE_VAL;
}
