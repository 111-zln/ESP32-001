#include "Rgb.h"

Rgb::Rgb(gpio_num_t r, gpio_num_t g, gpio_num_t b): r_(r), g_(g), b_(b)
{
    // 全局构造阶段不做硬件操作
}

void Rgb::init()
{
    r_.setOutput();
    g_.setOutput();
    b_.setOutput();
}

void Rgb::red()
{
    r_.write(0);
    g_.write(1);
    b_.write(1);
}

void Rgb::green()
{
    r_.write(1);
    g_.write(0);
    b_.write(1);
}

void Rgb::blue()
{
    r_.write(1);
    g_.write(1);
    b_.write(0);
}

void Rgb::off()
{
    r_.write(0);
    g_.write(0);
    b_.write(0);
}

void Rgb::on()
{
    r_.write(1);
    g_.write(1);
    b_.write(1);
}