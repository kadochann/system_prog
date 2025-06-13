#pragma once
#include <gpiod.hpp>

class Relay {
public:
    Relay();
    ~Relay();

    void init();
    void release();
    void set(bool value);

private:
    struct gpiod_chip* chip;
    struct gpiod_line* line;
    int gpioPin;
};
