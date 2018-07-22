#include "simple_math.h"

static const uint8_t sin_lut_data[] = {128, 176, 217, 245, 255, 245, 217, 176,
                                       128, 80,  39,  11,  1,   11,  39,  80};

uint8_t int_lerp(uint16_t a, uint16_t b, uint16_t i) {
    return (b * i / 256) + (a * (255 - i) / 256);
}

uint8_t int_trig(uint16_t x, const uint8_t* lut, const uint8_t lut_len) {
    uint16_t left_index = (x * lut_len / 256);
    uint16_t right_index = (left_index + 1) % lut_len;
    uint16_t remainder = (x - (left_index * 256 / lut_len)) * lut_len;
    uint16_t left = lut[left_index];
    uint16_t right = lut[right_index];
    return int_lerp(left, right, remainder);
}

uint8_t int_sin(uint8_t x) {
    return int_trig(x, sin_lut_data, ARR_SIZE(sin_lut_data));
}

uint8_t int_cos(uint8_t x) { return int_sin(x + 256 / 4); }
