#include "../bsp/Board.h"
#include "../bsp/Pins.hpp"


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
    button_.init();
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
    rgb_(Pin::LED_R, Pin::LED_G, Pin::LED_B),
    button_(Pin::SW3),
    lcd_()
{
}
