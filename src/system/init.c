/*
 * init.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */
#include <inc/device/UCS.h>
#include <inc/device/WDT.h>
#include <inc/device/TIMER.h>
#include <inc/device/ADC12.h>
#include <inc/device/LCD12864SPI.h>
//#include<inc/driver/GPIO_Config.h>

void device_init(void)
{
    UCS_init();
    TA0_init();
//    TA1_init();
    TA2_init();
    WDT_init();
    ADC12_init();
}

void driver_init(void)
{
    Ini_Lcd12864();
//    KEY_PRESS_init();
}



