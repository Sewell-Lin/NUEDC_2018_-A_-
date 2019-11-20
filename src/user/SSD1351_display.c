/*
 * SSD1351_display.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */
#include <math.h>
#include <MSP430F5529.h>
#include <stdio.h>
#include <string.h>

#include <inc/driver/SSD1351.h>
#include <inc/system/SSD1351_fonts.h>

#include <inc/user/DATA_process.h>
#include <inc/user/SSD1351_display.h>

//static display_data_t display_data = {
////    .mode = 1,
//    .flag = 0,
////    .index = 0b000,
////    .selected = 0b000
//};
//display_data_t *display = &display_data;

//void display_update(void)
//{
//    char str_buff[24];
//    memset(str_buff,0,sizeof(str_buff));
//
//    if(display->flag) {
//        snprintf(str_buff, sizeof(str_buff), "%6.1f",control->freq);
//        SSD1351_display_string(0, 0, str_buff, FONT_1608, Lime, Black);
//        display->flag = 0;
//    }
//}



