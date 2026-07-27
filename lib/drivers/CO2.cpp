#include "CO2.h"


bool Co2::init(I2c& i2c)
{
    i2c_ = &i2c;
    return true;
}

bool Co2::start()
{
    uint8_t cmd[2] = {0x21, 0xB1};     // start periodic measurement 开始周期性测量
    return i2c_->write(ADDRESS, cmd[0], &cmd[1], 1); 
}

int Co2::read()
{
    int16_t co2 ;
    float temp ;
    float humidity ;

    uint8_t cmd = 0xEC;             // read measurement 读出测量值
    uint8_t data[9];

    if (!i2c_->write(ADDRESS, cmd, nullptr, 0))
        return -1;

    delay(5); // SCD4x 需要时间

    if (!i2c_->read(ADDRESS, 0x00, data, 9))
        return -1;

    co2 = (data[0] << 8) | data[1];    //16位

    /*uint16_t rawT = (data[3] << 8) | data[4]; //16位
    uint16_t rawH = (data[6] << 8) | data[7]; //16位
    //这个传感器主要测co2，温湿度是附带的，没那么准
    temp = -45 + 175 * ((float)rawT / 65535.0f); 
    humidity = 100 * ((float)rawH / 65535.0f);*/

    return co2; //ppm
}