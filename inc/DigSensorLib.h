#ifndef DIGSENSORLIB_H
#define DIGSENSORLIB_H

#include <gpiod.hpp>
#include <string>

class DigSensor {
public:
    DigSensor();
    void init();
    void release();
    bool read();

private:
    struct gpiod_chip* chip;
    struct gpiod_line* lineSensor;
    std::string chipName;
};

#endif
