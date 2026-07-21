#include "../hal/UartHAL.h"

Uart::Uart(HardwareSerial& serial,int tx,int rx,uint32_t baud)
    : serial_(serial),
      tx_(tx),
      rx_(rx),
      baud_(baud)
{
}

void Uart::init()
{
    serial_.begin(baud_, SERIAL_8N1, rx_, tx_);
}

bool Uart::write(const uint8_t* data, size_t length)
{
    return serial_.write(data, length) == length;
}

size_t Uart::read(uint8_t* buffer, size_t length)
{
    size_t count = 0;

    while (count < length && serial_.available())
    {
        buffer[count++] = serial_.read();
    }

    return count;
}