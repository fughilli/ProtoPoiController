#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVO_COUNT (2)

typedef struct
{
    uint8_t pos[SERVO_COUNT];
} servo_ctl_t;

void servo_init(servo_ctl_t* control);

#endif // SERVO_H
