/*
 * GPIO_Config.c
 *
 *  Created on: 2018��7��17��
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
    //-----��ϻ�е�����������ڲ���������-----
//    P1DIR &= ~BIT1;                     //�趨P1.1Ϊ���
//    P1REN |=  BIT1;                     //����P1.1�ڲ�����������
//    P1OUT |=  BIT1;                     //����������Ϊ����
//
//    P2DIR &= ~BIT1;                     //�趨P2.1Ϊ���
//    P2REN |=  BIT1;                     //����P2.1�ڲ�����������
//    P2OUT |=  BIT1;                     //����������Ϊ����

//    P1DIR &= ~BIT2;                     //�趨P1.2Ϊ���
//    P1REN |=  BIT2;                     //����P1.2�ڲ�����������
//    P1OUT |=  BIT2;                     //����������Ϊ����
//
//    P1DIR &= ~BIT3;                     //�趨P1.3Ϊ���
//    P1REN |=  BIT3;                     //����P1.3�ڲ�����������
//    P1OUT |=  BIT3;                     //����������Ϊ����

//    P2DIR &= ~(BIT0 + BIT2 + BIT4 + BIT5);
//    P2REN |=  BIT0 + BIT2 + BIT4 + BIT5;
//    P2OUT |=  BIT0 + BIT2 + BIT4 + BIT5;
//}




