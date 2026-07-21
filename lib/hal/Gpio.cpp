#include "Gpio.h"


 Gpio::Gpio(gpio_num_t pin)
{
    pin_ = pin;

    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = 1ULL << pin;
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;   // 或 INPUT / OUTPUT
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;

    gpio_config(&io_conf);  
}


 void Gpio::setOutput()
{
   gpio_config_t io_conf = {};

   io_conf.pin_bit_mask = 1ULL<<pin_;           //设置GPIO位掩码
   io_conf.mode = GPIO_MODE_OUTPUT;             //设置为输出模式
   io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;//禁用下拉
   io_conf.pull_up_en = GPIO_PULLUP_DISABLE;    //禁用上拉
   io_conf.intr_type = GPIO_INTR_DISABLE ;      //禁用中断

   gpio_config(&io_conf); 
}


 void Gpio::setInput()
{
   gpio_config_t io_conf = {};

   io_conf.pin_bit_mask = 1ULL<<pin_;           //设置GPIO位掩码，为了了设置多个GPIO口，可以使用按位或运算
   io_conf.mode = GPIO_MODE_INPUT;             //设置为输入模式
   io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;//禁用下拉
   io_conf.pull_up_en = GPIO_PULLUP_DISABLE;    //禁用上拉
   io_conf.intr_type = GPIO_INTR_DISABLE ;      //禁用中断

   gpio_config(&io_conf); 
}


 void Gpio::write(bool level)
{
    gpio_set_level(pin_, level);
}


bool Gpio::read() const
{
    return gpio_get_level(pin_);
}


void Gpio::toggle() 
{
   bool currentlevel = read();
   write(!currentlevel);

}