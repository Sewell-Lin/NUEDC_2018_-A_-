/*
 * lcd12864_display.c
 *
 *  Created on: 2018Äê7ÔÂ21ÈÕ
 *      Author: Sewell Lin
 */
#include <math.h>
#include <MSP430F5529.h>
#include <stdio.h>
#include <string.h>

#include <inc/device/LCD12864SPI.h>

#include <inc/user/lcd12864_display.h>
#include <inc/user/DATA_process.h>

static display_data_t display_data = {
    .fok_flag = 0,
    .vpok_flag = 0
};
display_data_t *display = &display_data;

void display_update(void)
{
    if(display->fok_flag) {
        sprintf(tab_SPI1,"Fre: %.2f",control->freq);
        lcd12864_disp_str(1,1,tab_SPI1);
//        display->fok_flag = 0;
    }
    if(display->vpok_flag) {
        sprintf(tab_SPI2,"Vpp: %.2f",control->second);
        lcd12864_disp_str(1,2,tab_SPI2);

        sprintf(tab_SPI3,"Ipp: %.2f",control->I);
        lcd12864_disp_str(1,3,tab_SPI3);

        sprintf(tab_SPI4,"Vran: %.2f",control->second /2);
              lcd12864_disp_str(1,4,tab_SPI4);
        __delay_cycles(7820000);
//        display->vpok_flag = 0;
    }

}



