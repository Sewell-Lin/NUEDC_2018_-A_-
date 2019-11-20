/*
 * LCD12864.h
 *
 *  Created on: 2018Äê7ÔÂ21ÈÕ
 *      Author: Sewell Lin
 */

#ifndef INC_DEVICE_LCD12864SPI_H_
#define INC_DEVICE_LCD12864SPI_H_

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

void Write_Cmd(uchar cmd);
void  Write_Data(uchar dat);
uchar read_data();
void LCD_SetXY(uchar X, uchar Y);
void disp_char(uchar X, uchar Y, uchar wdata);
void lcd12864_disp_str(uchar X, uchar Y, char*Str);
void GUI_Point(unsigned char x,unsigned char y,unsigned char color);
void GUI_Fill_GDRAM(unsigned char dat);
void clrgdram();
void clrddram();
void LCD_Draw_Line_x( uchar X0, uchar X1, uchar Y, uchar Color );
void LCD_Draw_Line_Y( uchar X, uchar Y0, uchar Y1, uchar Color );
void LCD_Draw_Line( uchar StartX, uchar StartY, uchar EndX, uchar EndY, uchar Color );
void fsin2();
void Ini_Lcd12864(void);

extern char tab_SPI1[16];
extern char tab_SPI2[16];
extern char tab_SPI3[16];
extern char tab_SPI4[16];

//#define uchar unsigned char
//#define uint  unsigned int
//#define ulong unsigned long
//
//void Write_Cmd(uchar cmd);
//void  Write_Data(uchar dat);
//uchar read_data();
//void LCD_SetXY(uchar X, uchar Y);
//void disp_char(uchar X, uchar Y, uchar wdata);
//void lcd12864_disp_str(uchar X, uchar Y, uchar*Str);
//void GUI_Point(unsigned char x,unsigned char y,unsigned char color);
//void GUI_Fill_GDRAM(unsigned char dat);
//void clrgdram();
//void clrddram();
//void LCD_Draw_Line_x( uchar X0, uchar X1, uchar Y, uchar Color );
//void LCD_Draw_Line_Y( uchar X, uchar Y0, uchar Y1, uchar Color );
//void LCD_Draw_Line( uchar StartX, uchar StartY, uchar EndX, uchar EndY, uchar Color );
//void fsin2();
//void Ini_Lcd12864(void);

#endif /* INC_DEVICE_LCD12864SPI_H_ */
