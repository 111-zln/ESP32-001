#include "sensor_service.h"

Sensor sensorService ;

Sensor::Sensor()
    : bme280_(nullptr)
{
}


void Sensor::init(Bme280& bme280)
{
    bme280_ = &bme280;

    //co2_->start();   // 这里只调用一次
}

void Sensor::update()
{
    g_data.temp     = bme280_->readtemp();
    g_data.humidity = bme280_->readhumidity();
    g_data.pressure = bme280_->readpressure();
    //g_data.co2      = co2_->read();
}

float Sensor::getTemperature()
{
    return g_data.temp;
}

float Sensor::getHumidity()
{
    return g_data.humidity;
}

float Sensor::getPressure()
{
    return g_data.pressure;
}

/*int Sensor::getCo2()
{
    return g_data.co2;
}
*/  