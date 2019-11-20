/*
 * UCS.c
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
 *-------clock out-------
 *    ACLK:    P1.0
 *    SMCLK:   P2.2
 *    MCLK:    P7.7
 *-----------------------
 */
#include <msp430f5529.h>

void UCS_init(void)
{
/************************************************测量时钟频率*/

//  P1SEL |= BIT0; //ACLK
//  P1DIR |= BIT0;
//  P2SEL |= BIT2; //SMCLK
//  P2DIR |= BIT2;
//  P7SEL |= BIT7; //MCLK
//  P7DIR |= BIT7;

/*******************************************打开XT1，2外部晶振*/
    P5SEL |= BIT4|BIT5;
    UCSCTL6 |= XCAP_3;
    UCSCTL6 &= ~XT1OFF;//打开XT1，否则XT1LFOFFG可能报错

    P5SEL |= BIT2|BIT3;
    UCSCTL6 &= ~XT2OFF;//打开XT2，否则XT2OFFG可能报错

    __bis_SR_register(SCG0);//该语法为固定格式，意为将括号内的变量置位，SCG0与系统工作模式有关，此时MCLK暂停工作
/***************************************************配置DCO*/
        UCSCTL0 = 0x00;   //先清零，FLL运行时，该寄存器系统会自动配置，不用管
        UCSCTL1 = DCORSEL_6;//调节范围约为 ~ MHZ（设置DCO的频率范围，之后设置的DCO时钟要在这个范围内，否则会出错）
        UCSCTL2 = FLLD_1 | 243;//FLLD=1,FLLN=243,则频率为2*（243+1）*32.768=15.99MHZ
                               //DCOCLK = D*(N+1)*(REFCLK/n)
                               //DCOCLKDIV = (N+1)*(REFCLK/n)

    __bic_SR_register(SCG0);
    __delay_cycles(782000);//系统自带的精确延时，单位us

/************************************************等待晶振起振*/
    while (SFRIFG1 & OFIFG)
    {
            UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
            SFRIFG1 &= ~OFIFG;
    }

    UCSCTL4=(UCSCTL4&(~(SELA_7|SELS_7|SELM_7)))|SELA_0|SELS_5|SELM_3;
       //(UCSCTL4&(~(SELA_7|SELS_7|SELM_7)))先把SELA,SELS,SELM清零，然后设置各个时钟的来源
       //ACLK->X1COK(32.768KHz),SMCLK->X2CLK(4MHz),MCLK->DCOCLK(15.99MHZ)
}

//#define SET_ACLK_OUT    P1SEL |= BIT0; P1DIR |= BIT0    // 设置P1.0为ACLK输出
//#define SET_SMCLK_OUT   P2SEL |= BIT2; P2DIR |= BIT2    // 设置P2.2为SMCLK输出
//#define SET_MCLK_OUT    P7SEL |= BIT7; P7DIR |= BIT7    // 设置P7.7为MCLK输出
//
//static void set_vcore_up(unsigned int level)
//{
//    // Open PMM registers for write
//    PMMCTL0_H = PMMPW_H;
//    // Set SVS/SVM high side new level
//    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
//    // Set SVM low side to new level
//    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
//    // Wait till SVM is settled
//    while ((PMMIFG & SVSMLDLYIFG) == 0);
//    // Clear already set flags
//    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
//    // Set VCore to new level
//    PMMCTL0_L = PMMCOREV0 * level;
//    // Wait till new level reached
//    if ((PMMIFG & SVMLIFG)) {
//        while ((PMMIFG & SVMLVLRIFG) == 0);
//    }
//    // Set SVS/SVM low side to new level
//    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
//    // Lock PMM registers for write access
//    PMMCTL0_H = 0x00;
//}
//
//void UCS_init(void)
//{
////  SET_MCLK_OUT;     // 开启MCLK输出
////  SET_ACLK_OUT;     // 开启ACLK输出
////  SET_SMCLK_OUT;    // 开启SMCLK输出
//
////  Increase Vcore setting to level3 to support fsystem=25MHz
////  NOTE: Change core voltage one level at a time..
//    set_vcore_up(0x01);
//    set_vcore_up(0x02);
//    set_vcore_up(0x03);
//
//    UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFOCLK
//    UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
//
//    __bis_SR_register(SCG0);                  // Disable the FLL control loop
//    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
//    UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
//    UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 25MHz
//                                              // (N + 1) * FLLRef = Fdco
//                                              // (762 + 1) * 32768 = 25MHz
//                                              // Set FLL Div = fDCOCLK/2
//    __bic_SR_register(SCG0);                  // Enable the FLL control loop
//
//    // Worst-case settling time for the DCO when the DCO range bits have been
//    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
//    // UG for optimization.
//    // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
//    __delay_cycles(782000);
//
//    // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
//    do {
//        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);     // Clear XT2,XT1,DCO fault flags
//        SFRIFG1 &= ~OFIFG;                              // Clear fault flags
//    } while (SFRIFG1&OFIFG);                            // Test oscillator fault flag
//}
//
//
