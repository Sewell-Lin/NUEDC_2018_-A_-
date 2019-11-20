/*
 * lcd12864_display.h
 *
 *  Created on: 2018Äê7ÔÂ21ÈÕ
 *      Author: Sewell Lin
 */

#ifndef INC_USER_LCD12864_DISPLAY_H_
#define INC_USER_LCD12864_DISPLAY_H_

#include <stdint.h>

typedef struct {
//    uint8_t mode;
    uint8_t fok_flag;
    uint8_t vpok_flag;
//    uint8_t index;
//    uint8_t selected;
} display_data_t;

extern display_data_t *display;
extern void display_update(void);



#endif /* INC_USER_LCD12864_DISPLAY_H_ */
