/*
 * user.c
 *
 *  Created on: 2018��7��2��
 *      Author: Sewell Lin
 */
//#include<inc/user/KEY_input.h>
#include <inc/user/DATA_process.h>
#include <inc/user/lcd12864_display.h>

void user_update(void)
{
    data_update();
    display_update();
}
