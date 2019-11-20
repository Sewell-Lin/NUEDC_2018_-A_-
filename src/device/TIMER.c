/*
 * TIMER.c
 *
 *  Created on: 2018年7月13日
 *      Author: Sewell Lin
 *---------------------------
 *TA0     Capture
 *TA1     FFT_buf Sample
 *TA2     Sample time count
 *---------------------------
 */
#include <msp430f5529.h>
#include <stdint.h>
#include <intrinsics.h>

#include <inc/device/TIMER.h>
#include <inc/device/ADC12.h>

#include <inc/user/DATA_process.h>
//#include <inc/user/KEY_input.h>

static uint8_t up_flag = 0;
uint8_t overflow = 0;

void TA0_init(void)
{
//----------->>Capture
    P1SEL |= BIT2;
    P1DIR &= ~BIT2;
    TA0CCTL1 |= CM_1 + CCIS_0 + SCS + CAP + CCIE;
//----------->>Timer
//    TA0CCTL0 |= CCIE;
//    TA0CCR0 = ;

    TA0CTL |= TACLR + TASSEL_2 + MC_2 + TAIE;
}

void TA1_init(void)
{
//    TA1CCTL0 |= CCIE;
    TA1CTL |= TACLR + TASSEL_1 + MC_1 + TAIE;

    TA1CCR0 = 65535/Num;        //CCR0=T(contiue time)/Num(sample point)*F(Time frequence)
}

void TA2_init(void)
{
//    TA2CCTL0 |= CCIE;
    TA2CTL |= TACLR + TASSEL_1 + MC_1;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.  TA2CCR0 = ;
}
//--------------------->>采周期
void TA0_Cap_isr(void)
{
    static uint16_t up_o=0;                       //上升沿时间
    uint16_t up_t=0;

    if(TA0CCTL1&CCI) {
        //--------------->>捕获到上升沿并且采的周期,采样控制部分
        if(control->f_redy) {
            TA2CCR0 = control->T_switch;
            TA2CCTL0 |= CCIE;
        }
       // -------------->>频率测量部分
        if(up_flag) {
            TA0CCTL1 &= ~COV;
            up_t = TA0R;
            control->T_width = up_t + overflow * 65535 - up_o;
            control->f_ok = 1;
            up_t=0;
            up_o=0;
            up_flag = 0;
            overflow = 0;         //溢出计数清零
        } else {
            up_o = TA0R;       //first上升沿捕获
            up_flag = 1;
            overflow = 0;
        }
    }
}
//---------------------------------->>采样定时
inline void TA2_Compare_isr(void)
{
    __disable_interrupt();
    TA2CCTL0 &= ~CCIE;
    TA2R = 0;
//    ADC12_on();
    ADC12CTL0 |= ADC12ENC + ADC12SC;            //Start Sample
    __enable_interrupt();
}
//--------------------------------->>FFT
inline void TA1_Compare_isr(void)
{
    __disable_interrupt();
    TA1CCTL0 &= ~CCIE;
    TA1R = 0;
    if(adc->count<Num)
        ADC12_on();            //Start Sample
    __enable_interrupt();
    TA1CCTL0 |= CCIE;
}
