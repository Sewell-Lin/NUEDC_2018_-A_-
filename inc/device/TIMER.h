/*
 * TIMER.h
 *
 *  Created on: 2018��7��20��
 *      Author: Sewell Lin
 */

#ifndef INC_DEVICE_TIMER_H_
#define INC_DEVICE_TIMER_H_


extern void TA0_init(void);
extern void TA1_init(void);
extern void TA2_init(void);

extern void TA0_Cap_isr(void);
extern void TA1_Compare_isr(void);
extern void TA2_Compare_isr(void);


#endif /* INC_DEVICE_TIMER_H_ */
