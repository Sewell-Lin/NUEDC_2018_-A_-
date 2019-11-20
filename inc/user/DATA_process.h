/*
 * DATA_process.h
 *
 *  Created on: 2018Äê7ÔÂ2ÈÕ
 *      Author: Sewell Lin
 */

#ifndef INC_USER_DATA_PROCESS_H_
#define INC_USER_DATA_PROCESS_H_

#include <stdint.h>

typedef struct {
    double second;
    uint8_t f_ok;
    uint8_t vp_ok;
    uint8_t f_redy;
    int T_switch;
    double freq;
    double T_width;
    double Vpp;
    double Vp;
    double vp_buf[3];
    double I;
    double vran;
} control_data_t;

extern control_data_t *control;

extern void data_update(void);

#endif /* INC_USER_DATA_PROCESS_H_ */
