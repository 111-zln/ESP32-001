#include "Board.h"
#include "Pins.hpp"

void Board::init()
{
    Serial.println("  SPI init...");
    spi_.init();

    Serial.println("  I2C init...");
    i2c_.init();

    Serial.println("  UART wire init...");
    uart_wire.init();

    Serial.println("  UART UCB init...");
    uart_ucb.init();

    Serial.println("  RGB off...");
    rgb_.off();

    Serial.println("  LCD init...");
    lcd_.init();

    Serial.println("  BME280 init...");
    bool bmeOK = bme_.init(i2c_);
    Serial.println(bmeOK ? "  BME280 OK" : "  BME280 FAIL");

    /* ========== 关键：同步按键初始状态，否则边沿检测永远失效 ========== */
    Serial.println("  Button init...");
    up_.init();
    down_.init();
    left_.init();
    right_.init();
    ok_.init();
    key1_.init();
    key2_.init();
    Serial.println("  Button OK");
    /* ================================================================ */

    Serial.println("  Board init finished");
}

Board::Board()
    : i2c_(Pin::I2C_SDA, Pin::I2C_SCL),
      spi_(Pin::SPI_SCLK, Pin::SPI_MISO, Pin::SPI_MOSI),
      uart_wire(Serial2, Pin::Wire_Tx, Pin::Wire_Rx, 115200),
      uart_ucb(Serial1, Pin::Ucb_Tx, Pin::Ucb_Rx, 115200),
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