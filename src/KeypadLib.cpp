#include "KeypadLib.h"
#include <iostream>
#include <algorithm>

// Constructor
Keypad::Keypad() {
    chipName = "gpiochip0";  // default chip name
}

// Initialize GPIO lines
void Keypad::init() {
    chip = gpiod_chip_open_by_name(chipName.c_str());
    for (int i = 0; i < 4; i++) {
        lineKeyCol[i] = gpiod_chip_get_line(chip, keyGPIOCol[i]);
        lineKeyRow[i] = gpiod_chip_get_line(chip, keyGPIORow[i]);
    }
    for (int i = 0; i < 4; i++) {
        gpiod_line_request_output(lineKeyCol[i], "COL", 0);
    }
    for (int i = 0; i < 4; i++) {
        gpiod_line_request_input(lineKeyRow[i], "ROW");
    }
}

// Release GPIO lines
void Keypad::release() {
    for (int i = 0; i < 4; i++) {
        gpiod_line_release(lineKeyCol[i]);
        gpiod_line_release(lineKeyRow[i]);
    }
    gpiod_chip_close(chip);
}

// Read pressed key
char Keypad::getKey() {
    for (int col = 0; col < 4; col++) {
        gpiod_line_set_value(lineKeyCol[col], 1);
        for (int row = 0; row < 4; row++) {
            if (gpiod_line_get_value(lineKeyRow[row]) == 1) {
                gpiod_line_set_value(lineKeyCol[col], 0);
                return key[row][col];
            }
        }
        gpiod_line_set_value(lineKeyCol[col], 0);
    }
    return 'n';  // no key pressed
}
