/*
 * ADC12.c
 *
 *  Created on: 2018年7月20日
 *      Author: Sewell Lin
 */
#include <msp430f5529.h>
#include <stdint.h>

#include <inc/device/ADC12.h>

#include <inc/user/DATA_process.h>
#include <inc/user/lcd12864_display.h>

//const int16_t Num = 4096;

static adc_data_t adc_data = {
    .count = 0,         //采样计数位
//    .adc_buf = 0
};
adc_data_t *adc = &adc_data;

float adc_buf[Num]={0.0};

void ADC12_init(void)
{
    P6SEL |= BIT0;                                  //Sample Channel

    ADC12CTL0 &= ~ADC12ENC;                         //关转换
    REFCTL0 &= ~REFMSTR;
    ADC12CTL0 |= ADC12ON + ADC12SHT02 + ADC12REFON;         //开基准，默认1.5v
    ADC12MCTL0 |= ADC12INCH_0 + ADC12SREF_1;        //通道0，参考电源AVCC
    ADC12CTL1 |= ADC12SHP;                          //单通道单次采样，SMCLK，存入ADC12MEM0寄存器
    ADC12IE |= ADC12IE0;


//    P6SEL |= BIT0;          //P6.0,A0采样通道
//
//    ADC12CTL0 &= ~ADC12ENC;
//    ADC12CTL0 |= ADC12ON + ADC12SHT02;
//    ADC12MCTL0 |= ADC12INCH_0 + ADC12SREF_0;         //通道0，参考电源AVCC
//    ADC12CTL1 |= ADC12SHP;         //单通道单次采样，SMCLK，存入ADC12MEM0寄存器
//    ADC12IE |= ADC12IE0;
}

void ADC12_on(void)
{
    ADC12CTL0 |= ADC12ENC + ADC12SC;            //Start Sample
}

inline void ADC12_Vp_isr(void)
{
    control->Vp = ADC12MEM0;
//    display->vpok_flag = 1;
    control->vp_ok = 1;
    ADC12CTL0 &= ~ADC12ENC;
}

//inline void ADC12_fft_isr(void)
//{
//        adc->adc_buf[adc->count] = ADC12MEM0;
//        adc->count++;
//        adc_buf[count] = ADC12MEM0;
//        ADC12CTL0 &= ~ADC12ENC;
//}



