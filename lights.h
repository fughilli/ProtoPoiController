#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

#define LIGHTS_COUNT (3)

typedef struct { uint8_t level[LIGHTS_COUNT]; } lights_ctl_t;
typedef void (*tick_callback_t)(void);

void lights_init(lights_ctl_t* control, tick_callback_t callback);

#endif  // SERVO_H
