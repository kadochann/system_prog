#include "../inc/RelayLib.h"
#include <iostream>

Relay::Relay() : chip(nullptr), line(nullptr), gpioPin(27) {}

Relay::~Relay() {
    release();
}

void Relay::init() {
    chip = gpiod_chip_open_by_name("gpiochip0");
    line = gpiod_chip_get_line(chip, gpioPin);
    if (gpiod_line_request_output(line, "Relay", 0) < 0) {
        std::cerr << "[Relay] Failed to request output line\n";
    }
}

void Relay::set(bool value) {
    if (line)
        gpiod_line_set_value(line, value ? 1 : 0);  // aktif yüksek mi düşük mü burada test gerekebilir
}

void Relay::release() {
    if (line)
        gpiod_line_release(line);
    if (chip)
        gpiod_chip_close(chip);
}
