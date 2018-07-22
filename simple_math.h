/*
 * simple_math.h
 *
 *  Created on: Jul 20, 2012
 *      Author: Kevin
 *
 *
 * Copyright (C) 2012-2018  Kevin Balke
 */

#ifndef SIMPLE_MATH_H_
#define SIMPLE_MATH_H_

#include <stdint.h>

#define ARR_SIZE(l) (sizeof(l) / sizeof(l[0]))

#define lesser(a, b) (((a) < (b)) ? (a) : (b))
#define greater(a, b) (((a) > (b)) ? (a) : (b))
#define distance(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define clip(a, b, t) (((t) < (a)) ? (a) : ((((t) > (b)) ? (b) : (t))))

uint8_t int_sin(uint8_t x);
uint8_t int_cos(uint8_t x);
uint8_t int_trig(uint16_t x, const uint8_t* lut, const uint8_t lut_len);
uint8_t int_lerp(uint16_t a, uint16_t b, uint16_t i);

#endif /* SIMPLE_MATH_H_ */
