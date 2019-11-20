/*
 * WDT.c
 *
 *  Created on: 2018��7��2��
 *      Author: Sewell Lin
 */
#include <msp430f5529.h>

#include <inc/user/lcd12864_display.h>

void WDT_init(void)
{
//    WDTCTL = WDT_MDLY_0_5;
//    SFRIE1 |= WDTIE;
    //-----�趨WDTΪ16ms�ж�-----
   WDTCTL =WDT_ADLY_1000;
    //-----WDT�ж�ʹ��-----
   SFRIE1 |= WDTIE;
}

inline void WDT_isr(void)
{
    display->fok_flag = 1;
    display->vpok_flag = 1;
}
