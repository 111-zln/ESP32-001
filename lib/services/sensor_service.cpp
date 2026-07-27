#include "sensor_service.h"

Sensor::Sensor()
    : bme280_(nullptr),
      co2_(nullptr)
{
}

void Sensor::init(Bme280& bme280, Co2& co2)
{
    bme280_ = &bme280;
    co2_ = &co2;
}

void Sensor::update()
{
    // 后面统一读取传感器
}

float Sensor::getTemperature()
{
    return bme280_->readtemp();
}

float Sensor::getHumidity()
{
    return bme280_->readhumidity();
}

float Sensor::getPressure()
{
    return bme280_->readpressure();
}

int Sensor::getCo2()
{
    co2_ -> start();
    
    return co2_->read();
}