/*
 * USCI_B0_SPI.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
*  ---------SPI---------
 *  MODE    TYPE     PIN
 *  MOSI    OUT      P3.0
 *  MISO    IN       P3.1
 *  CLK     OUT      P3.2
 *  ---------------------
 */

#include <msp430f5529.h>

#define SPI_PIN_SET()   {\
                            P3SEL |= BIT0 + BIT1 + BIT2;\
                        }

static unsigned char *spi_tx_buff;
static unsigned char *spi_rx_buff;

static unsigned char spi_tx_num = 0;
static unsigned char spi_rx_num = 0;

unsigned char USCI_B0_SPI_transmit_frame(unsigned char *p_buff, unsigned char num)
{
    if (UCB0STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB0IE |= UCTXIE;
    UCB0IE &= ~UCRXIE;
    __enable_interrupt();
    spi_tx_buff = p_buff;
    spi_tx_num  = num;
    UCB0TXBUF = *spi_tx_buff++;
    while (UCB0STAT & UCBUSY);
    return 1;
}

unsigned char USCI_B0_SPI_receive_frame(unsigned char *p_buff, unsigned char num)
{
    if (UCB0STAT & UCBUSY) return 0;
    __disable_interrupt();
    UCB0IE &= ~UCTXIE;
    UCB0IE |= UCRXIE;
    __enable_interrupt();
    spi_rx_buff = p_buff;
    spi_rx_num = num;
    UCB0TXBUF = 0xff;
    while (UCB0STAT & UCBUSY);
    return 1;
}

void USCI_B0_SPI_init(void)
{
    SPI_PIN_SET();

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 |= UCMST + UCMODE_0 + UCSYNC + UCCKPL + UCMSB;
    UCB0CTL1 |= UCSSEL__SMCLK;

    UCB0BR0  = 2;
    UCB0BR1  = 0;

    UCB0CTL1 &= ~UCSWRST;
    UCB0IFG &= ~(UCTXIFG + UCRXIFG);
}

inline void USCI_B0_SPI_rx_isr_handle(void)
{
    *spi_rx_buff++ = UCB0RXBUF;
    spi_rx_num--;
    if (spi_rx_num) {
        UCB0TXBUF = 0xff;
    } else {
        UCB0IFG &= ~UCRXIFG;
        UCB0IE &= ~UCRXIE;
    }
}

inline void USCI_B0_SPI_tx_isr_handle(void)
{
    UCB0RXBUF;
    spi_tx_num--;
    if (spi_tx_num) {
        UCB0TXBUF = *spi_tx_buff++;
    } else {
        UCB0IFG &= ~UCTXIFG;
        UCB0IE &= ~UCTXIE;
    }
}




