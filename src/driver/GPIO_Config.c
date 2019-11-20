/*
 * GPIO_Config.c
 *
 *  Created on: 2018年7月17日
 *      Author: Sewell Lin
 *---------KEY---------
 *PORT          FUNCTION
 *P2.0          confirm
 *P2.2          bit select
 *P2.4          up
 *P2.5          down
 *---------------------
 */
#include <msp430f5529.h>

//-------->>buton Config
//void KEY_PRESS_init(void)
//{
    //-----配合机械按键，启用内部上拉电阻-----
//    P1DIR &= ~BIT1;                     //设定P1.1为输出
//    P1REN |=  BIT1;                     //启用P1.1内部上下拉电阻
//    P1OUT |=  BIT1;                     //将电阻设置为上拉
//
//    P2DIR &= ~BIT1;                     //设定P2.1为输出
//    P2REN |=  BIT1;                     //启用P2.1内部上下拉电阻
//    P2OUT |=  BIT1;                     //将电阻设置为上拉

//    P1DIR &= ~BIT2;                     //设定P1.2为输出
//    P1REN |=  BIT2;                     //启用P1.2内部上下拉电阻
//    P1OUT |=  BIT2;                     //将电阻设置为上拉
//
//    P1DIR &= ~BIT3;                     //设定P1.3为输出
//    P1REN |=  BIT3;                     //启用P1.3内部上下拉电阻
//    P1OUT |=  BIT3;                     //将电阻设置为上拉

//    P2DIR &= ~(BIT0 + BIT2 + BIT4 + BIT5);
//    P2REN |=  BIT0 + BIT2 + BIT4 + BIT5;
//    P2OUT |=  BIT0 + BIT2 + BIT4 + BIT5;
//}




