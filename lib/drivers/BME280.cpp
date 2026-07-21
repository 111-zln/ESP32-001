#include "../drivers/BME280.h"


bool Bme280::init(I2c& i2c)
{
    i2c_ = &i2c; //把i2c地址用指针保存起来，后面可以直接调用i2c_

    uint8_t id = 0;

    i2c_->read(ADDRESS,0xD0,&id,1);

    return id == 0x60;
}

uint16_t  Bme280::readhumidityRAW()
{
    uint8_t data[2];

    i2c_->read(ADDRESS,0xFD,data,2);

    uint16_t adc_H = ((uint16_t)data[0]<<8)|data[1];

    return adc_H;

}

uint32_t  Bme280::readtempRAW()
{
    uint8_t data[3];

    i2c_->read(ADDRESS, 0xFA, data, 3);

    uint32_t adc_T =           //将三个字节的数据拼接成一个20位的adc值
    ((uint32_t)data[0] << 12) |
    ((uint32_t)data[1] << 4)  |
    (data[2] >> 4);

    return adc_T;
}

uint32_t  Bme280::readpressureRAW()
{
    uint8_t data[3];

    i2c_->read(ADDRESS,0xF7,data,3);

    uint32_t adc_P = ((uint32_t)data[0]<<12)|((uint32_t)data[1]<<4)|(data[2]>>4); 

    return adc_P;

}




float Bme280::readtemp()
{
    uint32_t adc_T = readtempRAW();

    uint8_t data1[2];
    uint8_t data2[2];
    uint8_t data3[2];

    i2c_->read(ADDRESS, 0x88, data1, 2);
    i2c_->read(ADDRESS, 0x8A, data2, 2);
    i2c_->read(ADDRESS, 0x8C, data3, 2);


    cal_.dig_T1 = ((uint16_t)data1[1]<<8)|data1[0];
    cal_.dig_T2 = ((uint16_t)data2[1]<<8)|data2[0];
    cal_.dig_T3 = ((uint16_t)data3[1]<<8)|data3[0];

    int32_t var1;
    int32_t var2;
    int32_t T;


    var1 = ((((adc_T>>3)- ((long signed int)cal_.dig_T1<<1)))* ((long signed int)cal_.dig_T2)) >> 11;

    var2 = ((( ((adc_T>>4)- ((long signed int)cal_.dig_T1))
       * ((adc_T>>4) - ((long signed int)cal_.dig_T1))) >> 12)
       * ((long signed int)cal_.dig_T3)) >> 14;


    t_fine_ = var1 + var2;

    T = (t_fine_* 5 + 128 ) >> 8 ;

    return T / 100.0f;//单位DegC

}

float Bme280::readhumidity()
{

    uint16_t adc_H = readhumidityRAW();

    uint8_t data1[1];
    uint8_t data2[2];
    uint8_t data3[1];
    uint8_t data4[2];
    uint8_t data5[2];
    uint8_t data6[1];

    i2c_->read(ADDRESS, 0xA1, data1, 1);
    i2c_->read(ADDRESS, 0xE1, data2, 2);
    i2c_->read(ADDRESS, 0xE3, data3, 1);
    i2c_->read(ADDRESS, 0xE4, data4, 2);
    i2c_->read(ADDRESS, 0xE5, data5, 2);
    i2c_->read(ADDRESS, 0xE7, data6, 1);

    cal_.dig_H1 = ((uint16_t)data1[0]);
    cal_.dig_H2 = ((uint16_t)data2[1]<<8)|data2[0];
    cal_.dig_H3 = ((uint16_t)data3[0]);
    cal_.dig_H4 =((int16_t)data4[0] << 4) |(data4[1] & 0x0F);
    cal_.dig_H5 =((int16_t)data5[1] << 4) |(data5[0] >> 4);
    cal_.dig_H6 = ((uint8_t)data6[0]);
    
    int32_t v_x1_u32r;
    int32_t H;

    v_x1_u32r = (t_fine_ - ((long signed int)76800));

    v_x1_u32r  = ((((adc_H << 14) - (((long signed int)cal_.dig_H4) << 20) - (((long signed int)cal_.dig_H5)
                *v_x1_u32r)) + ((long signed int)16384)) >> 15) * (((((((v_x1_u32r * ((long signed int)cal_.dig_H6)) >> 10)
                *(((v_x1_u32r * ((long signed int)cal_.dig_H3)) >> 11) + ((long signed int)32768))) >> 10 )
                +((long signed int)2097152)) * ((long signed int)cal_.dig_H2) + 8192) >> 14) ;

    v_x1_u32r = (v_x1_u32r-(((((v_x1_u32r >> 15) * (v_x1_u32r >> 15 )) >> 7)* (long signed int)cal_.dig_H1))>>4) ;

    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);

    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return ((long signed int)(v_x1_u32r>>12))/ 1024 ; //%RH

}

float Bme280::readpressure()
{
    int32_t adc_P = readpressureRAW();

    uint8_t data1[2];
    uint8_t data2[2];
    uint8_t data3[2];
    uint8_t data4[2];
    uint8_t data5[2];
    uint8_t data6[2];
    uint8_t data7[2];
    uint8_t data8[2];
    uint8_t data9[2];

    i2c_->read(ADDRESS, 0x8E, data1, 2);
    i2c_->read(ADDRESS, 0x90, data2, 2);
    i2c_->read(ADDRESS, 0x92, data3, 2);
    i2c_->read(ADDRESS, 0x94, data4, 2);
    i2c_->read(ADDRESS, 0x96, data5, 2);
    i2c_->read(ADDRESS, 0x98, data6, 2);
    i2c_->read(ADDRESS, 0x9A, data7, 2);
    i2c_->read(ADDRESS, 0x9C, data8, 2);
    i2c_->read(ADDRESS, 0x9E, data9, 2);

    cal_.dig_P1 = ((uint16_t)data1[1]<<8)|data1[0];
    cal_.dig_P2 = ((uint16_t)data2[1]<<8)|data2[0];
    cal_.dig_P3 = ((uint16_t)data3[1]<<8)|data3[0];
    cal_.dig_P4 = ((uint16_t)data4[1]<<8)|data4[0];
    cal_.dig_P5 = ((uint16_t)data5[1]<<8)|data5[0];
    cal_.dig_P6 = ((uint16_t)data6[1]<<8)|data6[0];
    cal_.dig_P7 = ((uint16_t)data7[1]<<8)|data7[0];
    cal_.dig_P8 = ((uint16_t)data8[1]<<8)|data8[0];
    cal_.dig_P9 = ((uint16_t)data9[1]<<8)|data9[0];

    int64_t var1, var2, p;

    var1 = ((int64_t)t_fine_) - 128000;

    var2 = var1 * var1 * (int64_t)cal_.dig_P6;
    var2 = var2 + ((var1 * (int64_t)cal_.dig_P5) << 17);
    var2 = var2 + (((int64_t)cal_.dig_P4) << 35);

    var1 = ((var1 * var1 * (int64_t)cal_.dig_P3) >> 8) +
           ((var1 * (int64_t)cal_.dig_P2) << 12);

    var1 = (((((int64_t)1) << 47) + var1) *
            ((int64_t)cal_.dig_P1)) >> 33;

    if (var1 == 0)
    {
        return 0.0f;      // 防止除零
    }

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;

    var1 = (((int64_t)cal_.dig_P9) * (p >> 13) * (p >> 13)) >> 25;

    var2 = (((int64_t)cal_.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) +
        (((int64_t)cal_.dig_P7) << 4);

    // Pa
    return p / 256.0f;
}