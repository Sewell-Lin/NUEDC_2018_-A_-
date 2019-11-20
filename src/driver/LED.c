/*
 * LED.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */
#include <msp430f5529.h>

void LED_on(void)
{
    P1OUT |= BIT0;
}

void LED_off(void)
{
    P1OUT &=~BIT0;
}

void LED_init(void)
{
    P1DIR |= BIT0;
}
