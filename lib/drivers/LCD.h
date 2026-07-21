#pragma once

#include <U8g2lib.h>

class Lcd
{
    public:
 
        void init(); //开启u8g2，设置字体和编码

        void print(int x, int y, const char* str); //发送字符串到缓冲区

        void clear(); //清理缓冲区

        void send(); //发送缓冲区内容到显示屏

        
};
extern U8G2_ST7567_ENH_DG128064_F_4W_HW_SPI u8g2_;