#include "state_machine.h"

#include "msp430.h"
#include "simple_math.h"

#include <stdbool.h>

const static rgb_t red_rgb[] = {{255, 0, 0, 1000}};
DECLARE_SEQ(red);
const static rgb_t green_rgb[] = {{0, 255, 0, 1000}};
DECLARE_SEQ(green);
const static rgb_t blue_rgb[] = {{0, 0, 255, 1000}};
DECLARE_SEQ(blue);
const static rgb_t stripes_rgb[] = {{255, 0, 0, 1000}, {0, 255, 0, 1000}};
DECLARE_SEQ(stripes);
const static rgb_t plaid_rgb[] = {{255, 255, 0, 100},
                                  {255, 0, 0, 100},
                                  {255, 255, 0, 100},
                                  {0, 0, 255, 1000}};
DECLARE_SEQ(plaid);

void set_lights_rgb(lights_ctl_t* lights, const rgb_t* rgb) {
    lights->level[0] = rgb->g;
    lights->level[1] = rgb->b * 2 / 3;
    lights->level[2] = rgb->r;
}

static uint8_t debounce_reg = 0;

bool button_pressed() {
    debounce_reg = (debounce_reg << 1);
    debounce_reg |= (P2IN & 0x80) ? 1 : 0;
    return debounce_reg == 0x7F;
}

void update_rainbow(lights_ctl_t* lights) {
    static uint16_t counter = 0;
    static uint8_t idx_counter = 0;
    counter++;
    if (counter == 1000) {
        counter = 0;
        idx_counter++;
        for (uint16_t i = 0; i < 3; i++) {
            lights->level[i] = lights->level[i] =
                int_sin(i * 256 / 3 + idx_counter);
        }
    }
}

void state_machine_init(state_machine_t* state) {
    /* P2.7 input, pulldown */
    P2DIR &= ~0x80;
    P2REN |= 0x80;
    P2OUT &= ~0x80;
    P2SEL &= ~0xC0;
    P2SEL2 &= ~0xC0;

    state->cur_seq = &stripes_seq;
    state->cur_rgb = stripes_seq.rgb;
    state->state = STRIPES;
    state->cur_idx = 0;
    state->counter = 0;
}

void state_machine_tick(state_machine_t* state, lights_ctl_t* lights) {
    if (button_pressed()) {
        if (state->state == RAINBOW) {
            state->state = 0;
        } else {
            state->state++;
        }

        switch (state->state) {
            case RED:
                state->cur_seq = &red_seq;
                break;
            case GREEN:
                state->cur_seq = &green_seq;
                break;
            case BLUE:
                state->cur_seq = &blue_seq;
                break;
            case STRIPES:
                state->cur_seq = &stripes_seq;
                break;
            case PLAID:
                state->cur_seq = &plaid_seq;
                break;
            default:
                break;
        }

        state->cur_idx = 0;
        state->cur_rgb = state->cur_seq->rgb;
    }

    if (state->state == RAINBOW) {
        update_rainbow(lights);
        return;
    }

    state->counter++;
    if (state->counter >= state->cur_rgb->ticks) {
        state->cur_idx = (state->cur_idx + 1) % state->cur_seq->len;
        state->cur_rgb = &state->cur_seq->rgb[state->cur_idx];
        state->counter = 0;
    }
    set_lights_rgb(lights, state->cur_rgb);
}
