#include "i2c.h"
#include <wiringPiI2C.h>

int read_word(int device_handle, int address) {
    int high = wiringPiI2CReadReg8(device_handle, address);
    int low = wiringPiI2CReadReg8(device_handle, address + 1);
    return (high << 8) + low;
}
