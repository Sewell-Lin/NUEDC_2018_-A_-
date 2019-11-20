/*
 * SSD1351.h
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
 */

#ifndef INC_DRIVER_SSD1351_H_
#define INC_DRIVER_SSD1351_H_

enum SSD1351_scroll_direction {
    Left    = 0x01,
    Right   = 0x81
};

enum SSD1351_scroll_interval {
    Fast    = 0x00,
    Normal  = 0x01,
    Slow    = 0x02,
    Slowest = 0x03
};

enum SSD1351_data_type {
    SSD1351_CMD  = 0,
    SSD1351_DATA = 1
};

enum SSD1351_panel_value {
    SSD1351_WIDTH  = 128,
    SSD1351_HEIGHT = 128
};

enum SSD1351_fundamental_command_table {
    SET_COLUMN_ADDRESS  = 0x15,     //设置列地址
    SET_ROW_ADDRESS     = 0x75,     //设置行地址

    SET_WRITE_RAM       = 0x5C,
    SET_READ_RAM        = 0x5D,

    SET_REMAP_COLOR_DEPTH   = 0xA0,
    SET_DISPLAY_START_LINE  = 0xA1,
    SET_DISPLAY_OFFSET      = 0xA2,

    SET_ENTIRE_DISPLAY_OFF  = 0xA4,
    SET_ENTIRE_DISPLAY_ON   = 0xA5,
    SET_NORMAL_DISPLAY      = 0xA6,
    SET_INVERSE_DISPLAY     = 0xA7,

    SET_FUNCTION_SELECTION  = 0xAB,

    SET_SLEEP_MODE_ON       = 0xAE,
    SET_SLEEP_MODE_OFF      = 0xAF,

    SET_RESET_PRECHARGE_PERIOD      = 0xB1,
    SET_DISPLAY_ENHANCEMENT         = 0xB2,
    SET_FRONT_CLOCK_DIV_OSC_FREQ    = 0xB3,
    SET_SEGMENT_LOW_VOLTAGE         = 0xB4,
    SET_GPIO                        = 0xB5,
    SET_SECOND_PRECHARGE_PERIOD     = 0xB6,

    SET_GRAY_SCALE_TABLE    = 0xB8,
    SET_BUILTIN_LINEAR_LUT  = 0xB9,
    SET_PRECHARGE_VOLTAGE   = 0xBB,
    SET_VCOMH_VOLTAGE       = 0xBE,

    SET_CONTRAST_CURRENT    = 0xC1,
    SET_MASTER_CURRENT      = 0xC7,
    SET_MUX_RATIO           = 0xCA,

    SET_COMMAND_LOCK    = 0xFD
};

enum SSD1351_graphic_acceleration_command_table {
    HORIZONTAL_SCROLLING_SETUP  = 0x96,
    DEACTIVATE_SCROLLING        = 0x9E,
    ACTIVATE_SCROLLING          = 0x9F
};

extern void SSD1351_draw_point(unsigned char chXpos, unsigned char chYpos, unsigned int hwColor);
extern void SSD1351_draw_line(unsigned char chXpos0, unsigned char chYpos0, unsigned char chXpos1, unsigned char chYpos1, unsigned int hwColor);
extern void SSD1351_draw_h_line(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned int hwColor);
extern void SSD1351_draw_v_line(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor);
extern void SSD1351_draw_column(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor0, unsigned int hwColor1);
extern void SSD1351_draw_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor);
extern void SSD1351_draw_circle(unsigned char chXpos, unsigned char chYpos, unsigned char chRadius, unsigned int hwColor);
extern void SSD1351_draw_mono_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight, unsigned int hwForeColor, unsigned int hwBackColor);
extern void SSD1351_draw_64k_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight);
extern void SSD1351_fill_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor);
extern void SSD1351_fill_gram(unsigned int hwColor);
extern void SSD1351_clear_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight);
extern void SSD1351_clear_gram(void);
extern void SSD1351_display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
extern void SSD1351_display_num(unsigned char chXpos, unsigned char chYpos, unsigned long chNum, unsigned char chLen, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
extern void SSD1351_display_string(unsigned char chXpos, unsigned char chYpos, const char *pchString, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor);
extern void SSD1351_horizontal_scrolling(unsigned char chYpos, unsigned char chHeight, unsigned char chDirection, unsigned char chInterval);
extern void SSD1351_deactivate_scrolling(void);
extern void SSD1351_show_checkerboard(void);
extern void SSD1351_show_rainbow(void);

extern void SSD1351_init(void);



#endif /* INC_DRIVER_SSD1351_H_ */
