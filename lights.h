#ifndef LIGHT_H
#define LIGHT_H

#include <stdint.h>

#define LIGHTS_COUNT (2)

typedef struct
{
    uint8_t level[LIGHTS_COUNT];
} lights_ctl_t;

void lights_init(lights_ctl_t* control);

#endif // SERVO_H
