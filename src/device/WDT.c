/*
 * WDT.c
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
 */
#include <msp430f5529.h>

#include <inc/user/lcd12864_display.h>

void WDT_init(void)
{
//    WDTCTL = WDT_MDLY_0_5;
//    SFRIE1 |= WDTIE;
    //-----设定WDT为16ms中断-----
   WDTCTL =WDT_ADLY_1000;
    //-----WDT中断使能-----
   SFRIE1 |= WDTIE;
}

inline void WDT_isr(void)
{
    display->fok_flag = 1;
    display->vpok_flag = 1;
}
