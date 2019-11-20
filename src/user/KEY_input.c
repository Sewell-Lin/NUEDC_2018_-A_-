/*
 * KEY_input.c
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
*---------KEY---------
 *PORT          FUNCTION
 *P2.1          Stat Sample
 *P1.1          Clear
 *P2.4          up
 *P2.5          down
 *---------------------
 */
#include <stdint.h>
#include <msp430f5529.h>

#include <inc/driver/GPIO_Config.h>

#include <inc/device/TIMER.h>

#include <inc/user/KEY_input.h>
#include <inc/user/DATA_process.h>
#include <inc/user/SSD1351_display.h>

void P12_Onclick(void)
{
}

void P13_Onclick(void)
{
}

void P11_Onclick(void)
{
}

void P21_Onclick(void)
{
}

void KEY_IODect(void)
{
    static unsigned char KEY11_Now=0,KEY21_Now=0,KEY12_Now=0,KEY13_Now=0;
    unsigned char KEY11_Past=0,KEY21_Past=0,KEY12_Past=0,KEY13_Past=0;

    KEY11_Past = KEY11_Now;
    KEY21_Past = KEY21_Now;
    KEY12_Past = KEY12_Now;
    KEY13_Past = KEY13_Now;

    //-----查询IO的输入寄存器-----
    if((P1IN&BIT1)!=0)
        KEY11_Now=1;
    else
        KEY11_Now=0;

    if((P2IN&BIT1)!=0)
        KEY21_Now=1;
    else
        KEY21_Now=0;

    if((P1IN&BIT2)!=0)
        KEY12_Now=1;
    else
        KEY12_Now=0;

    if((P1IN&BIT3)!=0)
        KEY13_Now=1;
    else
        KEY13_Now=0;

    //-----前一次高电平、后一次低电平，说明按键按下-----
    if((KEY11_Past==1)&&(KEY11_Now==0)) P11_Onclick();
    if((KEY21_Past==1)&&(KEY21_Now==0)) P21_Onclick();
    if((KEY12_Past==1)&&(KEY12_Now==0)) P12_Onclick();
    if((KEY13_Past==1)&&(KEY13_Now==0)) P13_Onclick();

}

//void input_update(void)
//{
//    static uint8_t count[6] = {0};
//
//    if (!(P2IN & BIT0))
//    {
//        if (count[0]++ == 2)
//        {
//            count[0] = 0;
//            input_select();
//        }
//    }
//    else if (!(P2IN & BIT2))
//    {
//        if (count[1]++ == 2)
//        {
//            count[1] = 0;
//            input_confirm();
//        }
//    }
//    else if (!(P2IN & BIT4))
//    {
//        if (count[2]++ == 2)
//        {
//            count[2] = 0;
//            input_up();
//        }
//    }
//    else if (!(P2IN & BIT5))
//    {
//        if (count[3]++ == 2)
//        {
//            count[3] = 0;
//            input_down();
//        }
//    }
//}




