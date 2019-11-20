/*
 * ADC12.h
 *
 *  Created on: 2018Äê7ÔÂ20ÈÕ
 *      Author: Sewell Lin
 */

#ifndef INC_DEVICE_ADC12_H_
#define INC_DEVICE_ADC12_H_

#include <stdint.h>

#define Num 1024

typedef struct {
    uint8_t count;
//    double adc_buf[4096];
} adc_data_t;

extern adc_data_t *adc;

extern void ADC12_on(void);
extern void ADC12_init(void);
extern void ADC12_Vp_isr(void);
extern void ADC12_fft_isr(void);

#endif /* INC_DEVICE_ADC12_H_ */
