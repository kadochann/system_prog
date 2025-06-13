#include "DigSensorLib.h"
#include <unistd.h>

DigSensor::DigSensor() {
    chipName = "gpiochip0";
}

void DigSensor::init() {
    chip = gpiod_chip_open_by_name(chipName.c_str());
    lineSensor = gpiod_chip_get_line(chip, 17); // GPIO 17
    gpiod_line_request_input(lineSensor, "Sensor");
}

bool DigSensor::read() {
    return gpiod_line_get_value(lineSensor) == 1;
}

void DigSensor::release() {
    gpiod_line_release(lineSensor);
    gpiod_chip_close(chip);
}
