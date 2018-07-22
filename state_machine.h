#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "lights.h"

typedef enum { RED, GREEN, BLUE, STRIPES, PLAID, RAINBOW } color_states_t;

typedef struct {
    uint8_t r, g, b;
    uint16_t ticks;
} rgb_t;

typedef struct {
    const rgb_t* rgb;
    uint16_t len;
} seq_t;

typedef struct {
    color_states_t state;
    const seq_t* cur_seq;
    const rgb_t* cur_rgb;
    uint16_t cur_idx;
    uint16_t counter;
} state_machine_t;

#define DECLARE_SEQ(name)                               \
    const static seq_t name##_seq = {                   \
        .rgb = name##_rgb, .len = ARR_SIZE(name##_rgb), \
    }

void state_machine_tick(state_machine_t* state, lights_ctl_t* lights);
void state_machine_init(state_machine_t* state);

#endif
