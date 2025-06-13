#include <iostream>
#include <unistd.h>
#include <wiringPiI2C.h>
#include "../inc/GyroLib.h"
#include "../inc/SocketConLib.h"
#include "../inc/i2c.h"

int main() {
    int device_handle = wiringPiI2CSetup(I2C_DEVICE_ADDR);
    if (device_handle < 0) {
        std::cerr << "I2C init error\n";
        return -1;
    }

    set_gyro(device_handle, GYRO_FSCALE_250DPS);

    SocketCon socket;
    if (!socket.init("127.0.0.1", 7003, false)) {
        std::cerr << "Socket connection error\n";
        return -1;
    }

    while (true) {
        double gx = get_gyro_x(device_handle, GYRO_SENSITIVITY_250DPS);
        double gy = get_gyro_y(device_handle, GYRO_SENSITIVITY_250DPS);
        double gz = get_gyro_z(device_handle, GYRO_SENSITIVITY_250DPS);

        std::string message = "GYRO:" + std::to_string(gx) + "," +
                                         std::to_string(gy) + "," +
                                         std::to_string(gz);

        socket.send(message);
        sleep(1);
    }

    socket.release();
    return 0;
}
