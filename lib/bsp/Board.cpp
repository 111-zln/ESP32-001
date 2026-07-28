#include "../bsp/Board.h"
#include "../bsp/Pins.hpp"

Board board_;

void Board::init()
{
    // 1. 初始化 HAL
    spi_.init();
    i2c_.init();
    uart_wire.init();
    uart_ucb.init();

    // 2. 初始化驱动
    rgb_.off();
    lcd_.init();   // 重点：共享 SPI

    up_.init();
    down_.init();
    left_.init();
    right_.init();
    ok_.init();
    key1_.init();
    key2_.init();

    bme_.init(i2c_);
    co2_.init(i2c_);

    // 3. 初始化服务
    sensor_.init(bme_, co2_);

    // 4. 启动提示
    rgb_.green();
}

//初始化列表
Board::Board(): 

    //hal
    i2c_(Pin::I2C_SDA, Pin::I2C_SCL),
    spi_(Pin::SPI_SCLK, Pin::SPI_MISO, Pin::SPI_MOSI),
    uart_wire(Serial2,Pin::Wire_Tx,Pin::Wire_Rx,115200),
    uart_ucb(Serial1,Pin::Ucb_Tx ,Pin::Ucb_Rx ,115200),

    //drivers
    up_(Pin::SW4_UP),
    down_(Pin::SW4_DOWN),
    left_(Pin::SW4_LEFT),
    right_(Pin::SW4_RIGHT),
    ok_(Pin::SW4_OK),
    key1_(Pin::SW3),
    key2_(Pin::SW5),

    rgb_(Pin::LED_1, Pin::LED_2, Pin::LED_3),
    lcd_()
{
}
