#pragma once

// === GYRO REGISTER DEFINITIONS ===
#define GYRO_CONFIG_ADDR     0x1B
#define GYRO_XOUT_H_ADDR     0x43
#define GYRO_YOUT_H_ADDR     0x45
#define GYRO_ZOUT_H_ADDR     0x47

// === GYRO FULL SCALE RANGE DEFINITIONS ===
#define GYRO_FSCALE_250DPS   0x00
#define GYRO_FSCALE_500DPS   0x01
#define GYRO_FSCALE_1000DPS  0x02
#define GYRO_FSCALE_2000DPS  0x03

// === GYRO SENSITIVITY VALUES ===
#define GYRO_SENSITIVITY_250DPS   131.0
#define GYRO_SENSITIVITY_500DPS   65.5
#define GYRO_SENSITIVITY_1000DPS  32.8
#define GYRO_SENSITIVITY_2000DPS  16.4

// Function declarations
double scale_gyro(int raw_gyro, double sensitivity);

void set_gyro(int device_handle, int scale_val);

double get_gyro_x(int device_handle, double sensitivity);
double get_gyro_y(int device_handle, double sensitivity);
double get_gyro_z(int device_handle, double sensitivity);
