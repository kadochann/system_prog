#include "GyroLib.h"
#include "i2c.h" // read_word fonksiyonu burada

#include <wiringPiI2C.h>
#include <iostream>

// Sensörün verdiği raw değeri derece/saniyeye çevir
double scale_gyro(int raw_gyro, double sensitivity) {
    if (raw_gyro >= 0x8000)
        raw_gyro = -((65535 - raw_gyro) + 1);
    return static_cast<double>(raw_gyro) / sensitivity;
}

// Gyroskopun ölçekleme ayarını yap
void set_gyro(int device_handle, int scale_val) {
    uint8_t config_value = (scale_val << 3); // Bit 3-4'e yazılıyor
    wiringPiI2CWriteReg8(device_handle, GYRO_CONFIG_ADDR, config_value);
}

// X ekseni verisini oku
double get_gyro_x(int device_handle, double sensitivity) {
    int raw = read_word(device_handle, GYRO_XOUT_H_ADDR);
    return scale_gyro(raw, sensitivity);
}

// Y ekseni verisini oku
double get_gyro_y(int device_handle, double sensitivity) {
    int raw = read_word(device_handle, GYRO_YOUT_H_ADDR);
    return scale_gyro(raw, sensitivity);
}

// Z ekseni verisini oku
double get_gyro_z(int device_handle, double sensitivity) {
    int raw = read_word(device_handle, GYRO_ZOUT_H_ADDR);
    return scale_gyro(raw, sensitivity);
}
