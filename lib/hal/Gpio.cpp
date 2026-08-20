#include "Gpio.h"

Gpio::Gpio(gpio_num_t pin)
{
    pin_ = pin;
    // 全局构造阶段不做任何硬件操作！
}

void Gpio::setOutput()
{
   gpio_config_t io_conf = {};
   io_conf.pin_bit_mask = 1ULL << pin_;
   io_conf.mode = GPIO_MODE_OUTPUT;
   io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
   io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
   io_conf.intr_type = GPIO_INTR_DISABLE;
   gpio_config(&io_conf); 
}

void Gpio::setInput()
{
   gpio_config_t io_conf = {};
   io_conf.pin_bit_mask = 1ULL << pin_;
   io_conf.mode = GPIO_MODE_INPUT;
   io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
   io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
   io_conf.intr_type = GPIO_INTR_DISABLE;
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