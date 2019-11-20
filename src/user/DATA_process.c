/*
 * DATA_process.c
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <inc/device/ADC12.h>

#include <inc/user/lcd12864_display.h>
#include <inc/user/DATA_process.h>

static control_data_t control_data = {
       .freq = 0.0,
       .f_redy = 0,
       .T_width = 0,
       .T_switch = 0,
       .Vp = 0.0,
       .Vpp = 0.0,
       .f_ok = 0,
       .vp_ok = 0,
       .second = 0.0,
       .I = 0.0,
       .vran = 0.0,
};
control_data_t *control = &control_data;

const double ratio_x_2 = 0.0;
const double ratio_x_1 = 1.0;
const double ratio_const = 0.0;



void data_update(void)
{
    static int i = 0;



    if(control->f_ok) {
        control->freq = 4000000 * 1.0 / (control->T_width);
        control->T_switch = 32768 / (int)control->freq / 4;

        control->f_redy = 1;    //频率采集完成
        control->f_ok = 0;      //清频率准备标志位
        display->fok_flag = 1;                    //在WDT中完成
    }
    if(control->vp_ok) {
        control->Vp = control->Vp / 4096 * 1.5 * 1000;

        if(control->freq < 52 )
                                {
                                    //y = 6.7849x + 2.852

                                    control->Vp = 6.7849 * control->Vp + 2.852;
//                                    control->vran = control->Vp /2;

                                }

                   else if(control->freq >= 52 &&  control->freq < 502 )
                                       {


                             //       y = 6.7596x - 12.738
                                    control->Vp = 6.7596 * control->Vp  - 12.738;
//                                    control->vran = control->Vp /2;
                                       }

                   else if(control->freq >= 502 &&  control->freq < 1020 )
                                          {
                                    //y = 6.7803x - 8.494

                                    control->Vp = 6.7803 * control->Vp  - 8.494;

//                                    control->vran = control->Vp /2;
                                          }

                                //特殊值处理
        if(control->freq > 998 )
        {
            if(control->Vp < 118.80)
                                           {
                           control->Vp= 100.03 ;
           //                control->vran = control->Vp /2;
                                           }
            else  if(control->Vp > 9975.00)
                                            {
                                  control->Vp= 10000.08 ;

            //                      control->vran = control->Vp /2;
                                  }
        }
        else if(control->freq < 998){
            if(control->Vp < 116.80)
                                {
                control->Vp= 100.03 ;
//                control->vran = control->Vp /2;
                                }
                  else  if(control->Vp > 9975.00)
                                {
                      control->Vp= 10000.08 ;

//                      control->vran = control->Vp /2;
                      }
        }

        if(i<3) {
            control->vp_buf[i] = control->Vp;
            i++;
        } else {
            double max = control->vp_buf[0] > control->vp_buf[1] ? control->vp_buf[0] : control->vp_buf[1];
            max = max > control->vp_buf[2] ? max : control->vp_buf[2];
            double min = control->vp_buf[0] < control->vp_buf[1] ? control->vp_buf[0]: control->vp_buf[1];
            min = min < control->vp_buf[2] ? min : control->vp_buf[2];
            control->second = control->vp_buf[0] + control->vp_buf[1] + control->vp_buf[2] - max - min;
            i = 0;
        }
        //F=50HZ,



        //        control->Vpp = 2 * control->Vp;
        control->Vpp = control->Vp;

        control->I = control->second / 10;


        display->vpok_flag = 1;
    }
}




