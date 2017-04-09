#include <msp430.h>

#include "servo.h"
#include "util/numeric_utils.h"

#define SMCLK_FREQ (16000000 / 4)
#define TA_FREQ (SMCLK_FREQ / 2)
#define TA_PERIOD (TA_FREQ / 50)
#define SERVO_MINVAL (TA_FREQ / 1000)
#define SERVO_RANGE (SERVO_MINVAL)
#define SERVO_SCALE_VAL NU_DIV_ROUND_UP(SERVO_RANGE, 256)

static servo_ctl_t* servo_ctl;

void servo_init(servo_ctl_t* control)
{
    servo_ctl = control;

    TA0CTL = TACLR;
    TA0CTL = TASSEL_2 | ID_1 | TAIE;

    TA0CCR0 = TA_PERIOD - 1;
    TA0CCR1 = TA0CCR2 = SERVO_MINVAL;

    TA0CCTL1 = TA0CCTL2 = OUTMOD_7;

    /* Configure P3.5 as TA0.1 and P3.6 as TA0.2 */
    P3DIR |= 0x60;
    P3SEL |= 0x60;
    P3SEL2 &= ~0x60;

    TA0CTL |= MC_1;
}

__attribute__((__interrupt__(TIMER0_A1_VECTOR)))
void ISR_timer0_a1()
{
    TA0CTL &= ~TAIFG;

    TA0CCR1 = SERVO_MINVAL + servo_ctl->pos[0] * SERVO_SCALE_VAL;
    TA0CCR2 = SERVO_MINVAL + servo_ctl->pos[1] * SERVO_SCALE_VAL;
}
