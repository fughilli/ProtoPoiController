#include <msp430.h>

#include "i2c_memdev.h"

typedef enum
{
    I2CS_IDLE = 0,
    I2CS_STTX,
    I2CS_TX,
    I2CS_STRX,
    I2CS_RXADDR,
    I2CS_RXADDR_DONE,
    I2CS_RX
} i2c_state_e;

static struct {
    uint8_t *writemem;
    const uint8_t *readmem;
    uint16_t writelen, readlen, idx;
    i2c_state_e state;
} i2c_state;

void i2c_init_writemem(uint8_t* mem, uint8_t len)
{
    i2c_state.writemem = mem;
    i2c_state.writelen = (mem) ? (len) : 0;
}

void i2c_init_readmem(const uint8_t* mem, uint8_t len)
{
    i2c_state.readmem = mem;
    i2c_state.readlen = (mem) ? (len) : 0;
}

void i2c_init_mem(slvaddr_t slave_addr)
{
    UCB0CTL1 = UCSWRST;

    UCB0CTL0 = UCMODE_3 | UCSYNC;

    UCB0I2COA = slave_addr;

    UCB0I2CIE = UCSTTIE | UCSTPIE | UCNACKIE;

    i2c_state.state = I2CS_IDLE;

    UCB0CTL1 &= ~UCSWRST;

    IE2 = UCB0TXIE | UCB0RXIE;

    /* Select USCI mode for P1.6 and P1.7 */
    P1SEL |= 0xC0;
    P1SEL2 |= 0xC0;
}

__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
void ISR_usci_i2c_state()
{
    if(UCB0STAT & UCSTPIFG)
    {
        i2c_state.state = I2CS_IDLE;
        UCB0STAT &= ~UCSTPIFG;
    }

    if(UCB0STAT & UCSTTIFG)
    {
        if(UCB0CTL1 & UCTR)
        {
            /* Start in slave transmitter mode */
            if(i2c_state.state == I2CS_RXADDR_DONE)
            {
                i2c_state.state = I2CS_TX;
            }
            else
            {
                i2c_state.state = I2CS_IDLE;
            }
        }
        else
        {
            /* Start in slave receiver mode */
            i2c_state.state = I2CS_RXADDR;
        }
        UCB0STAT &= ~UCSTTIFG;
    }

    if(UCB0STAT & UCNACKIFG)
    {
        /* Don't transmit anything else this transaction */
        i2c_state.idx = i2c_state.readlen;

        UCB0STAT &= ~UCNACKIFG;
    }
}

__attribute((__interrupt__(USCIAB0TX_VECTOR)))
void ISR_usci_i2c_rxtx()
{
    if(IFG2 & UCB0TXIFG)
    {
        /* Transmit flag */
        if(i2c_state.idx < i2c_state.readlen)
        {
            UCB0TXBUF = i2c_state.readmem[i2c_state.idx++];
        }
        else
        {
            UCB0TXBUF = 0;
        }
    }

    if(IFG2 & UCB0RXIFG)
    {
        /* Receive flag */
        switch(i2c_state.state)
        {
            case I2CS_RXADDR:
                i2c_state.idx = UCB0RXBUF;
                i2c_state.state = I2CS_RXADDR_DONE;
                break;

            case I2CS_RXADDR_DONE:
                i2c_state.state = I2CS_RX;

            case I2CS_RX:
                if(i2c_state.idx < i2c_state.writelen)
                {
                    i2c_state.writemem[i2c_state.idx++] = UCB0RXBUF;
                }

                if(i2c_state.idx >= i2c_state.writelen)
                {
                    UCB0CTL1 |= UCTXNACK;
                }
                break;

            default:
                /* Should not get here */
                break;
        }
    }
}

