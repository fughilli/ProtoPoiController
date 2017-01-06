#ifndef I2C_MEMDEV_H
#define I2C_MEMDEV_H

#include <stdint.h>

typedef uint8_t slvaddr_t;

void i2c_init_mem(slvaddr_t slave_addr);
void i2c_init_readmem(const uint8_t* mem, uint8_t len);
void i2c_init_writemem(uint8_t* mem, uint8_t len);

#endif // I2C_MEMDEV_H
