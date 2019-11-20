/*
 * Vector.c
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */
#include<msp430f5529.h>
#include<stdint.h>

#include<inc/device/WDT.h>
#include<inc/device/TIMER.h>
#include<inc/device/ADC12.h>

//#include <inc/user/KEY_input.h>

extern uint8_t overflow;

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    WDT_isr();
//    KEY_IODect();
//    __bic_SR_register_on_exit(LPM0_bits);
}
//

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
    switch(__even_in_range(TA0IV,14))
    {
      case 2:
          TA0_Cap_isr();
          break;
      case 14:
          overflow++;
          break;
      default:
          break;
    }
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    TA1_Compare_isr();
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void)
{
    TA2_Compare_isr();
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
  switch(__even_in_range(ADC12IV,34))
  {
  case ADC12IV_ADC12IFG0:
      ADC12_Vp_isr();
      break;
//  case ADC12IV_ADC12IFG1:
//      ADC12_fft_isr();
//      break;
  default:
      break;
  }
}



