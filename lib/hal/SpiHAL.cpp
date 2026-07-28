#include "SpiHAL.h"

Spi::Spi(int sck,int miso, int mosi)
    : sck_(sck),miso_(miso),mosi_(mosi)
{
}

void Spi::init()
{
    SPI.begin(sck_,miso_,mosi_);
}

bool Spi::write(const uint8_t* data,size_t length)
{
    SPI.writeBytes(data,length);

    return true;
}

bool Spi::transfer(const uint8_t* tx,uint8_t* rx,size_t length)
{
    SPI.transferBytes(const_cast<uint8_t*>(tx), rx, length);

    return true;
}