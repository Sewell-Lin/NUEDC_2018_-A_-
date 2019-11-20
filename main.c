/*
 * main.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */
#include <MSP430F5529.h>

#include <inc/system/init.h>
#include <inc/system/user.h>
char STR0[17]={"Fre:          Hz"};
char STR1[17]={"Vpp:          mV"};
char STR2[17]={"Ipp:          mA"};
char STR3[17]={"Vran:         mV"};

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    device_init();
    driver_init();
    __enable_interrupt();

    while(1) {
        user_update();

        lcd12864_disp_str(1,1,STR0);
        lcd12864_disp_str(1,2,STR1);
        lcd12864_disp_str(1,3,STR2);
        lcd12864_disp_str(1,4,STR3);
    }
}



