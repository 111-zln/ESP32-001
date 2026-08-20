#include "Lcd.h"
#include "Pins.hpp"
#include <SPI.h>

U8G2_ST7567_ENH_DG128064_F_4W_HW_SPI u8g2_(U8G2_R0,Pin::LCD_CS,Pin::LCD_DC,Pin::LCD_RST);


void Lcd::init()
{
    SPI.begin(
        Pin::SPI_SCLK,
        Pin::SPI_MISO,
        Pin::SPI_MOSI,
        Pin::LCD_CS
    );
    u8g2_.begin();
    u8g2_.enableUTF8Print(); // 设置为UTF-8编码，以支持中文字符
    u8g2_.setFont(u8g2_font_wqy12_t_chinese2); // 设置字体为支持中文的字体
    u8g2_.setFontPosTop(); // 设置字体位置为顶部对齐
}

void Lcd::clear()
{
    u8g2_.clearBuffer();
}

void Lcd::print(int x,int y,const char* str)
{
    u8g2_.drawStr(x,y,str);
}

void Lcd::send()
{
    u8g2_.sendBuffer();
}