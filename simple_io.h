/*
 * simple_io.h
 *
 *  Created on: Jul 11, 2012
 *      Author: Kevin
 *
 *
 * Copyright (C) 2012-2018  Kevin Balke
 */

#ifndef SIMPLE_IO_H_
#define SIMPLE_IO_H_

#include "global_const.h"

#if (CLOCK_SPEED_MHz == 16)
#define CLOCK_SPEED 16000000  // 16*(1000^2)	16MHz
#define CLOCK_TIME_MS 16000   // CLOCK_SPEED/1000
#elif (CLOCK_SPEED_MHz == 1)
#define CLOCK_SPEED 1000000  // 1000^2			1MHz
#define CLOCK_TIME_MS 1000   // CLOCK_SPEED/1000
#endif

// Initialize PORT1 pins
#define INIT_PORT1() \
    do {             \
        P1OUT = 0;   \
        P1DIR = 0;   \
    } while (0)

// Initialize PORT2 pins
#define INIT_PORT2() \
    do {             \
        P2OUT = 0;   \
        P2DIR = 0;   \
    } while (0)

// Set PORT1 pin to high
#define set_pin(pin)                           \
    do {                                       \
        P1OUT |= (1 << pin) & 0xFF;            \
        P2OUT |= (((1 << pin) & 0xFF00) >> 8); \
    } while (0)

// Set PORT1 pin to low
#define clear_pin(pin)                         \
    do {                                       \
        P1OUT &= ~(1 << pin) & 0xFF;           \
        P2OUT &= ~(((1 << pin) & 0xFF00) >> 8) \
    } while (0)

// Read the digital level of the PORT1 pin
#define get_pin(pin) \
    (pin >= 8) ? ((P2IN >> (pin - 8)) & 1) : ((P1IN >> pin) & 1)

// Set PORT1 pin to be a digital output
#define set_pin_output(pin)                    \
    do {                                       \
        P1DIR |= (1 << pin) & 0xFF;            \
        P2DIR |= (((1 << pin) & 0xFF00) >> 8); \
    } while (0)

// Set PORT1 pin to be a digital input
#define set_pin_input(pin)                      \
    do {                                        \
        P1DIR &= ~(1 << pin) & 0xFF;            \
        P2DIR &= ~(((1 << pin) & 0xFF00) >> 8); \
    } while (0)

// Set PORT1 pin to be an ADC10 analog input
#define set_pin_analog_input(pin)                   \
    do {                                            \
        ADC10AE0 |= 1 << ping P1DIR &= ~(1 << pin); \
    } while (0)

// Select the ADC10 input channel
#define select_analog_channel(pin)                        \
    do {                                                  \
        ADC10CTL1 &= ~0xF000g ADC10CTL1 |= pin * 0x1000u; \
    } while (0)

void delay(long ms);
void delay_ticks(long ticks);

#endif /* SIMPLE_IO_H_ */
