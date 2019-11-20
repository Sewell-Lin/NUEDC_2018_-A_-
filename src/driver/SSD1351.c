/*
 * SSD1351.c
 *
 *  Created on: 2018年7月2日
 *      Author: Sewell Lin
 * --------SSD1351--------
 * PORT     TYPE    PIN
 * DC       OUT     P3.7
 * CS       OUT     P1.4
 * RES      OUT     P3.6
 * CLK      OUT     P3.2(SPI)->SCL
 * MOSI     OUT     P3.0(SPI)->SDA
 * -----------------------
 */
#include <MSP430F5529.h>

#include <inc/driver/SSD1351.h>
#include <inc/device/USCI_B0_SPI.h>
#include <inc/system/SSD1351_fonts.h>

#define abs(x) ((x)>0?(x):-(x))
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define SSD1351_PIN_SET()   P3DIR |= BIT6; P3DIR |= BIT7; P1DIR |= BIT4

#define SSD1351_DC_SET()    P3OUT |= BIT7
#define SSD1351_DC_CLR()    P3OUT &= ~BIT7

#define SSD1351_CS_SET()    P1OUT |= BIT4
#define SSD1351_CS_CLR()    P1OUT &= ~BIT4

#define SSD1351_RES_SET()   P3OUT |= BIT6
#define SSD1351_RES_CLR()   P3OUT &= ~BIT6

#define SSD1351_WRITE_BYTE(__DATA)  USCI_B0_SPI_transmit_frame(&__DATA, 1)

static void SSD1351_write_byte(unsigned char chData, unsigned char chCmd)
{
    if (chCmd == SSD1351_DATA) {
        SSD1351_DC_SET();
    } else {
        SSD1351_DC_CLR();
    }

    SSD1351_CS_CLR();
    SSD1351_WRITE_BYTE(chData);
    SSD1351_CS_SET();
}
/*
 * function:
 * 点亮屏幕上的一个像素点。
 */
void SSD1351_draw_point(unsigned char chXpos, unsigned char chYpos, unsigned int hwColor)
{
    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    SSD1351_write_byte(hwColor >> 8, SSD1351_DATA);
    SSD1351_write_byte(hwColor, SSD1351_DATA);
}

void SSD1351_draw_line(unsigned char chXpos0, unsigned char chYpos0, unsigned char chXpos1, unsigned char chYpos1, unsigned int hwColor)
{
    int x = chXpos1 - chXpos0;
    int y = chYpos1 - chYpos0;
    int dx = abs(x), sx = chXpos0 < chXpos1 ? 1 : -1;
    int dy = -abs(y), sy = chYpos0 < chYpos1 ? 1 : -1;
    int err = dx + dy, e2;

    if (chXpos0 >= SSD1351_WIDTH || chYpos0 >= SSD1351_HEIGHT || chXpos1 >= SSD1351_WIDTH || chYpos1 >= SSD1351_HEIGHT) {
        return;
    }

    for (;;){
        SSD1351_draw_point(chXpos0, chYpos0 , hwColor);
        e2 = 2 * err;
        if (e2 >= dy) {
            if (chXpos0 == chXpos1) break;
            err += dy; chXpos0 += sx;
        }
        if (e2 <= dx) {
            if (chYpos0 == chYpos1) break;
            err += dx; chYpos0 += sy;
        }
    }
}

void SSD1351_draw_h_line(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned int hwColor)
{
    unsigned int i, x1 = min(chXpos + chWidth, SSD1351_WIDTH- 1);

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos + x1 -1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < x1; i++) {
        SSD1351_write_byte(hwColor >> 8, SSD1351_DATA);
        SSD1351_write_byte(hwColor, SSD1351_DATA);
    }
}

void SSD1351_draw_v_line(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor)
{
    unsigned int i, y1 = min(chYpos + chHeight, SSD1351_HEIGHT - 1);

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos + y1 - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < y1; i++) {
        SSD1351_write_byte(hwColor >> 8, SSD1351_DATA);
        SSD1351_write_byte(hwColor, SSD1351_DATA);
    }
}

#pragma CODE_SECTION(SSD1351_draw_column, ".TI.ramfunc")
void SSD1351_draw_column(unsigned char chXpos, unsigned char chYpos, unsigned char chHeight, unsigned int hwColor0, unsigned int hwColor1)
{
    unsigned int i;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_WIDTH || chYpos+chHeight >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_HEIGHT - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = chYpos; i < chYpos+chHeight; i++) {
        SSD1351_write_byte(hwColor0 >> 8, SSD1351_DATA);
        SSD1351_write_byte(hwColor0, SSD1351_DATA);
    }
    for (i = chYpos+chHeight; i < SSD1351_HEIGHT; i++) {
        SSD1351_write_byte(hwColor1 >> 8, SSD1351_DATA);
        SSD1351_write_byte(hwColor1, SSD1351_DATA);
    }
}

void SSD1351_draw_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor)
{
    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_draw_h_line(chXpos, chYpos, chWidth, hwColor);
    SSD1351_draw_h_line(chXpos, chYpos + chHeight, chWidth, hwColor);
    SSD1351_draw_v_line(chXpos, chYpos, chHeight, hwColor);
    SSD1351_draw_v_line(chXpos + chWidth, chYpos, chHeight + 1, hwColor);
}

void SSD1351_draw_circle(unsigned char chXpos, unsigned char chYpos, unsigned char chRadius, unsigned int hwColor)
{
    int x = -chRadius, y = 0, err = 2 - 2 * chRadius, e2;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    do {
        SSD1351_draw_point(chXpos - x, chYpos + y, hwColor);
        SSD1351_draw_point(chXpos + x, chYpos + y, hwColor);
        SSD1351_draw_point(chXpos + x, chYpos - y, hwColor);
        SSD1351_draw_point(chXpos - x, chYpos - y, hwColor);
        e2 = err;
        if (e2 <= y) {
            err += ++y * 2 + 1;
            if(-x == y && e2 <= x)
                e2 = 0;
        }
        if(e2 > x)
            err += ++x * 2 + 1;
    } while(x <= 0);
}

void SSD1351_draw_mono_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight, unsigned int hwForeColor, unsigned int hwBackColor)
{
    unsigned char i, j, byteWidth = (chWidth + 7) / 8;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos + chWidth - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos + chHeight - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < chHeight; i++) {
        for (j = 0; j < chWidth; j++) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                SSD1351_write_byte(hwForeColor >> 8, SSD1351_DATA);
                SSD1351_write_byte(hwForeColor, SSD1351_DATA);
            }
            else {
                SSD1351_write_byte(hwBackColor >> 8, SSD1351_DATA);
                SSD1351_write_byte(hwBackColor, SSD1351_DATA);
            }
        }
    }
}

void SSD1351_draw_64k_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight)
{
    unsigned char i, j;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos + chWidth - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos + chHeight - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < chHeight; i++) {
        for (j = 0; j < chWidth; j++) {
            SSD1351_write_byte(*pchBmp++, SSD1351_DATA);
            SSD1351_write_byte(*pchBmp++, SSD1351_DATA);
        }
    }
}

void SSD1351_fill_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight, unsigned int hwColor)
{
    unsigned int i, j;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos + chWidth - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos + chHeight - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < chHeight; i++) {
        for (j = 0; j < chWidth; j++) {
            SSD1351_write_byte(hwColor >> 8, SSD1351_DATA);
            SSD1351_write_byte(hwColor, SSD1351_DATA);
        }
    }
}

void SSD1351_fill_gram(unsigned int hwColor)
{
    unsigned char i, j;

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_WIDTH - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_HEIGHT - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < SSD1351_HEIGHT; i ++) {
        for (j = 0; j < SSD1351_WIDTH; j ++) {
            SSD1351_write_byte(hwColor >> 8, SSD1351_DATA);
            SSD1351_write_byte(hwColor, SSD1351_DATA);
        }
    }
}

void SSD1351_clear_rect(unsigned char chXpos, unsigned char chYpos, unsigned char chWidth, unsigned char chHeight)
{
    unsigned int i, j;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chXpos, SSD1351_DATA);
    SSD1351_write_byte(chXpos + chWidth - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chYpos + chHeight - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < chHeight; i++) {
        for (j = 0; j < chWidth; j++) {
            SSD1351_write_byte(0x00, SSD1351_DATA);
            SSD1351_write_byte(0x00, SSD1351_DATA);
        }
    }
}

void SSD1351_clear_gram(void)
{
    unsigned char i, j;

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_WIDTH - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_HEIGHT - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i = 0; i < SSD1351_HEIGHT; i++) {
        for (j = 0; j < SSD1351_WIDTH; j++) {
            SSD1351_write_byte(0x00, SSD1351_DATA);
            SSD1351_write_byte(0x00, SSD1351_DATA);
        }
    }
}

void SSD1351_display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor)
{
    unsigned char i, j;
    unsigned char chTemp, chYpos0 = chYpos;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    for (i = 0; i < fonts_width[chFontIndex] * ((fonts_height[chFontIndex] + 7) / 8); i++) {
        switch (chFontIndex) {
        case FONT_1206:
            chTemp = c_chFont1206[chChr - ' '][i];
            break;
        case FONT_1608:
            chTemp = c_chFont1608[chChr - ' '][i];
            break;
        case FONT_1616:
            chTemp = c_chFont1616[chChr - ' '][i];
            break;
        case FONT_3216:
            chTemp = c_chFont3216[chChr - ' '][i];
            break;
        default:
            chTemp = 0x00;
            break;
        }

        for (j = 0; j < 8; j++) {
            if (chTemp & 0x80) {
                SSD1351_draw_point(chXpos, chYpos, hwForeColor);
            }
            else {
                SSD1351_draw_point(chXpos, chYpos, hwBackColor);
            }
            chTemp <<= 1;
            chYpos++;
            if ((chYpos - chYpos0) == fonts_height[chFontIndex]) {
                chYpos = chYpos0;
                chXpos++;
                break;
            }
        }
    }
}

static unsigned long _pow(unsigned char m, unsigned char n)
{
    unsigned long result = 1;
    while(n --)
        result *= m;
    return result;
}

void SSD1351_display_num(unsigned char chXpos, unsigned char chYpos, unsigned long chNum, unsigned char chLen, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor)
{
    unsigned char i;
    unsigned char chTemp, chShow = 0;

    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    for(i = 0; i < chLen; i++) {
        chTemp = (chNum / _pow(10, chLen - i - 1)) % 10;    //求余
        if(chShow == 0 && i < (chLen - 1)) {
            if(chTemp == 0) {
                SSD1351_display_char(chXpos + fonts_width[chFontIndex] * i, chYpos, ' ', chFontIndex, hwForeColor, hwBackColor);
                continue;
            } else {
                chShow = 1;
            }
        }
        SSD1351_display_char(chXpos + fonts_width[chFontIndex] * i, chYpos, chTemp + '0', chFontIndex, hwForeColor, hwBackColor);
    }
}

void SSD1351_display_string(unsigned char chXpos, unsigned char chYpos, const char *pchString, unsigned char chFontIndex, unsigned int hwForeColor, unsigned int hwBackColor)
{
    if (chXpos >= SSD1351_WIDTH || chYpos >= SSD1351_HEIGHT) {
        return;
    }

    while (*pchString != '\0') {
        if (chXpos > (SSD1351_WIDTH - fonts_width[chFontIndex])) {
            chXpos = 0;
            chYpos += fonts_height[chFontIndex];
            if (chYpos > (SSD1351_HEIGHT - fonts_height[chFontIndex])) {
                chYpos = chXpos = 0;
                SSD1351_clear_gram();
            }
        }

        SSD1351_display_char(chXpos, chYpos, *pchString, chFontIndex, hwForeColor, hwBackColor);
        chXpos += fonts_width[chFontIndex];
        pchString++;
    }
}

void SSD1351_horizontal_scrolling(unsigned char chYpos, unsigned char chHeight, unsigned char chDirection, unsigned char chInterval)
{
    if (chYpos >= SSD1351_WIDTH || (chYpos+chHeight) >= SSD1351_WIDTH) {
        return;
    }

    SSD1351_write_byte(HORIZONTAL_SCROLLING_SETUP, SSD1351_CMD);
    SSD1351_write_byte(chDirection, SSD1351_DATA);
    SSD1351_write_byte(chYpos, SSD1351_DATA);
    SSD1351_write_byte(chHeight, SSD1351_DATA);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(chInterval, SSD1351_DATA);

    SSD1351_write_byte(ACTIVATE_SCROLLING, SSD1351_CMD);
}

void SSD1351_deactivate_scrolling(void)
{
    SSD1351_write_byte(DEACTIVATE_SCROLLING, SSD1351_CMD);
}

void SSD1351_show_checkerboard(void)
{
    unsigned char i,j;

    SSD1351_write_byte(SET_COLUMN_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_WIDTH - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_ROW_ADDRESS, SSD1351_CMD);
    SSD1351_write_byte(0x00, SSD1351_DATA);
    SSD1351_write_byte(SSD1351_HEIGHT - 1, SSD1351_DATA);

    SSD1351_write_byte(SET_WRITE_RAM, SSD1351_CMD);
    for (i=0; i<SSD1351_HEIGHT/2; i++) {
        for (j=0; j<SSD1351_WIDTH/2; j++) {
            SSD1351_write_byte(0xFF, SSD1351_DATA);
            SSD1351_write_byte(0xFF, SSD1351_DATA);
            SSD1351_write_byte(0x00, SSD1351_DATA);
            SSD1351_write_byte(0x00, SSD1351_DATA);
        }
        for (j=0; j<SSD1351_WIDTH/2; j++) {
            SSD1351_write_byte(0x00, SSD1351_DATA);
            SSD1351_write_byte(0x00, SSD1351_DATA);
            SSD1351_write_byte(0xFF, SSD1351_DATA);
            SSD1351_write_byte(0xFF, SSD1351_DATA);
        }
    }
}

void SSD1351_show_rainbow(void)
{
    // White => Column 0~15
    SSD1351_fill_rect(0x00, 0x00, 0x10, SSD1351_HEIGHT, White);
    // Yellow => Column 16~31
    SSD1351_fill_rect(0x10, 0x00, 0x10, SSD1351_HEIGHT, Yellow);
    // Purple => Column 32~47
    SSD1351_fill_rect(0x20, 0x00, 0x10, SSD1351_HEIGHT, Magenta);
    // Cyan => Column 48~63
    SSD1351_fill_rect(0x30, 0x00, 0x10, SSD1351_HEIGHT, Cyan);
    // Red => Column 64~79
    SSD1351_fill_rect(0x40, 0x00, 0x10, SSD1351_HEIGHT, Red);
    // Green => Column 80~95
    SSD1351_fill_rect(0x50, 0x00, 0x10, SSD1351_HEIGHT, Lime);
    // Blue => Column 96~111
    SSD1351_fill_rect(0x60, 0x00, 0x10, SSD1351_HEIGHT, Blue);
    // Black => Column 112~127
    SSD1351_fill_rect(0x70, 0x00, 0x10, SSD1351_HEIGHT, Black);
}

static inline void SSD1351_set_gray_scale_table(void)
{
    SSD1351_write_byte(SET_GRAY_SCALE_TABLE, SSD1351_CMD);  // Set Pulse Width for Gray Scale Table
    SSD1351_write_byte(0x02, SSD1351_DATA);                 // Gray Scale Level 1
    SSD1351_write_byte(0x03, SSD1351_DATA);                 // Gray Scale Level 2
    SSD1351_write_byte(0x04, SSD1351_DATA);                 // Gray Scale Level 3
    SSD1351_write_byte(0x05, SSD1351_DATA);                 // Gray Scale Level 4
    SSD1351_write_byte(0x06, SSD1351_DATA);                 // Gray Scale Level 5
    SSD1351_write_byte(0x07, SSD1351_DATA);                 // Gray Scale Level 6
    SSD1351_write_byte(0x08, SSD1351_DATA);                 // Gray Scale Level 7
    SSD1351_write_byte(0x09, SSD1351_DATA);                 // Gray Scale Level 8
    SSD1351_write_byte(0x0A, SSD1351_DATA);                 // Gray Scale Level 9
    SSD1351_write_byte(0x0B, SSD1351_DATA);                 // Gray Scale Level 10
    SSD1351_write_byte(0x0C, SSD1351_DATA);                 // Gray Scale Level 11
    SSD1351_write_byte(0x0D, SSD1351_DATA);                 // Gray Scale Level 12
    SSD1351_write_byte(0x0E, SSD1351_DATA);                 // Gray Scale Level 13
    SSD1351_write_byte(0x0F, SSD1351_DATA);                 // Gray Scale Level 14
    SSD1351_write_byte(0x10, SSD1351_DATA);                 // Gray Scale Level 15
    SSD1351_write_byte(0x11, SSD1351_DATA);                 // Gray Scale Level 16
    SSD1351_write_byte(0x12, SSD1351_DATA);                 // Gray Scale Level 17
    SSD1351_write_byte(0x13, SSD1351_DATA);                 // Gray Scale Level 18
    SSD1351_write_byte(0x15, SSD1351_DATA);                 // Gray Scale Level 19
    SSD1351_write_byte(0x17, SSD1351_DATA);                 // Gray Scale Level 20
    SSD1351_write_byte(0x19, SSD1351_DATA);                 // Gray Scale Level 21
    SSD1351_write_byte(0x1B, SSD1351_DATA);                 // Gray Scale Level 22
    SSD1351_write_byte(0x1D, SSD1351_DATA);                 // Gray Scale Level 23
    SSD1351_write_byte(0x1F, SSD1351_DATA);                 // Gray Scale Level 24
    SSD1351_write_byte(0x21, SSD1351_DATA);                 // Gray Scale Level 25
    SSD1351_write_byte(0x23, SSD1351_DATA);                 // Gray Scale Level 26
    SSD1351_write_byte(0x25, SSD1351_DATA);                 // Gray Scale Level 27
    SSD1351_write_byte(0x27, SSD1351_DATA);                 // Gray Scale Level 28
    SSD1351_write_byte(0x2A, SSD1351_DATA);                 // Gray Scale Level 29
    SSD1351_write_byte(0x2D, SSD1351_DATA);                 // Gray Scale Level 30
    SSD1351_write_byte(0x30, SSD1351_DATA);                 // Gray Scale Level 31
    SSD1351_write_byte(0x33, SSD1351_DATA);                 // Gray Scale Level 32
    SSD1351_write_byte(0x36, SSD1351_DATA);                 // Gray Scale Level 33
    SSD1351_write_byte(0x39, SSD1351_DATA);                 // Gray Scale Level 34
    SSD1351_write_byte(0x3C, SSD1351_DATA);                 // Gray Scale Level 35
    SSD1351_write_byte(0x3F, SSD1351_DATA);                 // Gray Scale Level 36
    SSD1351_write_byte(0x42, SSD1351_DATA);                 // Gray Scale Level 37
    SSD1351_write_byte(0x45, SSD1351_DATA);                 // Gray Scale Level 38
    SSD1351_write_byte(0x48, SSD1351_DATA);                 // Gray Scale Level 39
    SSD1351_write_byte(0x4C, SSD1351_DATA);                 // Gray Scale Level 40
    SSD1351_write_byte(0x50, SSD1351_DATA);                 // Gray Scale Level 41
    SSD1351_write_byte(0x54, SSD1351_DATA);                 // Gray Scale Level 42
    SSD1351_write_byte(0x58, SSD1351_DATA);                 // Gray Scale Level 43
    SSD1351_write_byte(0x5C, SSD1351_DATA);                 // Gray Scale Level 44
    SSD1351_write_byte(0x60, SSD1351_DATA);                 // Gray Scale Level 45
    SSD1351_write_byte(0x64, SSD1351_DATA);                 // Gray Scale Level 46
    SSD1351_write_byte(0x68, SSD1351_DATA);                 // Gray Scale Level 47
    SSD1351_write_byte(0x6C, SSD1351_DATA);                 // Gray Scale Level 48
    SSD1351_write_byte(0x70, SSD1351_DATA);                 // Gray Scale Level 49
    SSD1351_write_byte(0x74, SSD1351_DATA);                 // Gray Scale Level 50
    SSD1351_write_byte(0x78, SSD1351_DATA);                 // Gray Scale Level 51
    SSD1351_write_byte(0x7D, SSD1351_DATA);                 // Gray Scale Level 52
    SSD1351_write_byte(0x82, SSD1351_DATA);                 // Gray Scale Level 53
    SSD1351_write_byte(0x87, SSD1351_DATA);                 // Gray Scale Level 54
    SSD1351_write_byte(0x8C, SSD1351_DATA);                 // Gray Scale Level 55
    SSD1351_write_byte(0x91, SSD1351_DATA);                 // Gray Scale Level 56
    SSD1351_write_byte(0x96, SSD1351_DATA);                 // Gray Scale Level 57
    SSD1351_write_byte(0x9B, SSD1351_DATA);                 // Gray Scale Level 58
    SSD1351_write_byte(0xA0, SSD1351_DATA);                 // Gray Scale Level 59
    SSD1351_write_byte(0xA5, SSD1351_DATA);                 // Gray Scale Level 60
    SSD1351_write_byte(0xAA, SSD1351_DATA);                 // Gray Scale Level 61
    SSD1351_write_byte(0xAF, SSD1351_DATA);                 // Gray Scale Level 62
    SSD1351_write_byte(0xB4, SSD1351_DATA);                 // Gray Scale Level 63
}

void SSD1351_init(void)
{
    SSD1351_PIN_SET();
    SSD1351_RES_SET();

    SSD1351_write_byte(SET_COMMAND_LOCK, SSD1351_CMD);              // Set Command Lock
    SSD1351_write_byte(0x12, SSD1351_DATA);                         // Unlock Driver IC
    SSD1351_write_byte(SET_COMMAND_LOCK, SSD1351_CMD);              // Set Command Lock
    SSD1351_write_byte(0xB1, SSD1351_DATA);                         // Unlock All Commands

    SSD1351_write_byte(SET_SLEEP_MODE_ON, SSD1351_CMD);             // Display Off

    SSD1351_write_byte(SET_FRONT_CLOCK_DIV_OSC_FREQ, SSD1351_CMD);  // Set Display Clock Divider / Oscillator Frequency
    SSD1351_write_byte(0xF0, SSD1351_DATA);                         // Set Clock as 90 Frames/Sec
    SSD1351_write_byte(SET_MUX_RATIO, SSD1351_CMD);                 // Set Multiplex Ratio
    SSD1351_write_byte(0x7F, SSD1351_DATA);                         // 1/128 Duty (0x0F~0x7F)
    SSD1351_write_byte(SET_DISPLAY_OFFSET, SSD1351_CMD);            // Set Vertical Scroll by Row
    SSD1351_write_byte(0x00, SSD1351_DATA);                         // Shift Mapping RAM Counter (0x00~0x7F)
    SSD1351_write_byte(SET_DISPLAY_START_LINE, SSD1351_CMD);        // Set Vertical Scroll by RAM
    SSD1351_write_byte(0x00, SSD1351_DATA);                         // Set Mapping RAM Display Start Line (0x00~0x7F)
    SSD1351_write_byte(SET_REMAP_COLOR_DEPTH, SSD1351_CMD);         // Set Re-Map / Color Depth
    SSD1351_write_byte(0x74, SSD1351_DATA);                         // Set Horizontal Address Increment
    //  Column Address 0 Mapped to SEG0
    //  Color Sequence D[15:0]=[RRRRR:GGGGGG:BBBBB]
    //  Scan from COM127 to COM0
    //  Enable COM Split Odd Even
    //  65,536 Colors Mode (0x74)
    //  * 262,144 Colors Mode (0xB4)
    SSD1351_write_byte(SET_GPIO, SSD1351_CMD);                      // General Purpose IO
    SSD1351_write_byte(0x00, SSD1351_DATA);                         // Disable GPIO Pins Input
    SSD1351_write_byte(SET_FUNCTION_SELECTION, SSD1351_CMD);        // Set Vertical Scroll by RAM
    SSD1351_write_byte(0x01, SSD1351_DATA);                         // Enable Internal VDD Regulator
    //  Select 8-bit Parallel Interface
    SSD1351_write_byte(SET_SEGMENT_LOW_VOLTAGE, SSD1351_CMD);       // Set Segment Low Voltage
    SSD1351_write_byte(0xA0, SSD1351_DATA);                         // Enable External VSL
    SSD1351_write_byte(0xB5, SSD1351_DATA);                         // Fixed
    SSD1351_write_byte(0x55, SSD1351_DATA);                         // Fixed
    SSD1351_write_byte(SET_CONTRAST_CURRENT, SSD1351_CMD);          // Set Contrast Current for Color A, B, C
    SSD1351_write_byte(0xC8, SSD1351_DATA);                         // Color A
    SSD1351_write_byte(0x80, SSD1351_DATA);                         // Color B
    SSD1351_write_byte(0xC8, SSD1351_DATA);                         // Color C
    SSD1351_write_byte(SET_MASTER_CURRENT, SSD1351_CMD);            // Master Contrast Current Control
    SSD1351_write_byte(0x0F, SSD1351_DATA);                         // Maximum

    SSD1351_write_byte(SET_BUILTIN_LINEAR_LUT, SSD1351_CMD);        // Default
    SSD1351_set_gray_scale_table();                                 // Set Pulse Width for Gray Scale Table

    SSD1351_write_byte(SET_RESET_PRECHARGE_PERIOD, SSD1351_CMD);    // Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
    SSD1351_write_byte(0x32, SSD1351_DATA);                         // Set Phase 1 as 5 Clocks & Phase 2 as 3 Clocks
    SSD1351_write_byte(SET_PRECHARGE_VOLTAGE, SSD1351_CMD);         // Set Pre-Charge Voltage Level
    SSD1351_write_byte(0x17, SSD1351_DATA);                         // Set Pre-Charge Voltage Level as 0.50*VCC
    SSD1351_write_byte(SET_DISPLAY_ENHANCEMENT, SSD1351_CMD);       // Display Enhancement
    SSD1351_write_byte(0xA4, SSD1351_DATA);                         // Enhance Display Performance
    SSD1351_write_byte(0x00, SSD1351_DATA);                         // Fixed
    SSD1351_write_byte(0x00, SSD1351_DATA);                         // Fixed
    SSD1351_write_byte(SET_SECOND_PRECHARGE_PERIOD, SSD1351_CMD);   // Set Second Pre-Charge Period
    SSD1351_write_byte(0x01, SSD1351_DATA);                         // Set Second Pre-Charge Period as 1 Clock
    SSD1351_write_byte(SET_VCOMH_VOLTAGE, SSD1351_CMD);             // Set COM Deselect Voltage Level
    SSD1351_write_byte(0x05, SSD1351_DATA);                         // Set Common Pins Deselect Voltage Level as 0.82*VCC
    SSD1351_write_byte(SET_NORMAL_DISPLAY, SSD1351_CMD);            // Normal Display Mode

    SSD1351_clear_gram();                                           //清屏

    SSD1351_write_byte(SET_SLEEP_MODE_OFF, SSD1351_CMD);            // Display On
}




