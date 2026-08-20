#pragma once

#include <driver/gpio.h>

namespace Pin
{
    // SPI
    constexpr gpio_num_t SPI_SCLK = GPIO_NUM_18;
    constexpr gpio_num_t SPI_MOSI = GPIO_NUM_23;
    constexpr int SPI_MISO =  -1;
 
    // LCD
    constexpr gpio_num_t LCD_CS  = GPIO_NUM_5;
    constexpr gpio_num_t LCD_DC  = GPIO_NUM_4;
    constexpr gpio_num_t LCD_RST = GPIO_NUM_25;

    // I2C
    constexpr gpio_num_t I2C_SDA = GPIO_NUM_21;
    constexpr gpio_num_t I2C_SCL = GPIO_NUM_22;

    // UART（1-Wire）&&(Ucb)
    constexpr gpio_num_t Wire_Tx = GPIO_NUM_17;
    constexpr gpio_num_t Wire_Rx = GPIO_NUM_16;
    constexpr gpio_num_t Ucb_Tx  = GPIO_NUM_9;
    constexpr gpio_num_t Ucb_Rx  = GPIO_NUM_10;

    // RGB LED
    constexpr gpio_num_t LED_1 = GPIO_NUM_8;
    constexpr gpio_num_t LED_2 = GPIO_NUM_7;
    constexpr gpio_num_t LED_3 = GPIO_NUM_6;

    // 按键
    constexpr gpio_num_t SW3 = GPIO_NUM_33;
    constexpr gpio_num_t SW5 = GPIO_NUM_2;

    constexpr gpio_num_t SW4_LEFT  = GPIO_NUM_36;
    constexpr gpio_num_t SW4_RIGHT = GPIO_NUM_39;
    constexpr gpio_num_t SW4_UP    = GPIO_NUM_34;
    constexpr gpio_num_t SW4_DOWN  = GPIO_NUM_35;
    constexpr gpio_num_t SW4_OK    = GPIO_NUM_32;
    
}