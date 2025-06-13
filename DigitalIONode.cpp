#include "../inc/SocketConLib.h"
#include "../inc/RelayLib.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <gpiod.hpp>

// === Tek tuşluk giriş (menü/röle için) ===
std::string scanKeypadOnce() {
    const char* chipname = "gpiochip0";
    int cols[4] = {6, 13, 19, 26};
    int rows[4] = {16, 20, 21, 12};
    char keyMap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) return "key err:";

    gpiod_line* lineCols[4];
    gpiod_line* lineRows[4];

    for (int i = 0; i < 4; ++i) {
        lineRows[i] = gpiod_chip_get_line(chip, rows[i]);
        gpiod_line_request_input(lineRows[i], "keypad_row");
    }

    for (int i = 0; i < 4; ++i) {
        lineCols[i] = gpiod_chip_get_line(chip, cols[i]);
        gpiod_line_request_output(lineCols[i], "keypad_col", 0);
    }

    // Önce: tuşların tamamen serbest kalmasını bekle
    int waitCycles = 30;
    while (waitCycles--) {
        bool pressed = false;
        for (int col = 0; col < 4; ++col) {
            gpiod_line_set_value(lineCols[col], 1);
            for (int row = 0; row < 4; ++row) {
                if (gpiod_line_get_value(lineRows[row]) == 1) {
                    pressed = true;
                }
            }
            gpiod_line_set_value(lineCols[col], 0);
        }
        if (!pressed) break;  // hepsi serbest bırakılmışsa çık
        usleep(100000); // 100ms
    }

    // Şimdi gerçek tuş basımını bekle
    int attempts = 50;
    while (attempts--) {
        for (int col = 0; col < 4; ++col) {
            gpiod_line_set_value(lineCols[col], 1);
            usleep(1000);

            for (int row = 0; row < 4; ++row) {
                int val = gpiod_line_get_value(lineRows[row]);
                if (val == 1) {
                    char key = keyMap[row][col];
                    gpiod_chip_close(chip);
                    return std::string("key ") + key + ":";
                }
            }

            gpiod_line_set_value(lineCols[col], 0);
        }

        usleep(100000);
    }

    gpiod_chip_close(chip);
    return "key x:";
}


// === # ile bitene kadar çoklu giriş ===
std::string scanKeypadString() {
    const char* chipname = "gpiochip0";
    int cols[4] = {6, 13, 19, 26};
    int rows[4] = {16, 20, 21, 12};
    char keyMap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) return "key err:";

    gpiod_line* lineCols[4];
    gpiod_line* lineRows[4];

    for (int i = 0; i < 4; ++i) {
        lineRows[i] = gpiod_chip_get_line(chip, rows[i]);
        gpiod_line_request_input(lineRows[i], "keypad_row");
    }

    for (int i = 0; i < 4; ++i) {
        lineCols[i] = gpiod_chip_get_line(chip, cols[i]);
        gpiod_line_request_output(lineCols[i], "keypad_col", 0);
    }

    std::string keyBuffer = "";
    int stableDelay = 300000;
    int maxTotalWait = 10000000;
    int waited = 0;

    std::cout << "[Keypad] Giriş bekleniyor... '#' ile bitirin.\n";

    while (waited < maxTotalWait) {
        for (int col = 0; col < 4; ++col) {
            gpiod_line_set_value(lineCols[col], 1);
            usleep(1000);

            for (int row = 0; row < 4; ++row) {
                int val = gpiod_line_get_value(lineRows[row]);
                if (val == 1) {
                    char key = keyMap[row][col];
                    std::cout << "[Keypad] Tuş: " << key << std::endl;
                    keyBuffer += key;
                    if (key == '#') {
                        gpiod_chip_close(chip);
                        return "key " + keyBuffer + ":";
                    }
                    usleep(stableDelay);
                }
            }

            gpiod_line_set_value(lineCols[col], 0);
        }

        usleep(50000);
        waited += 50000;
    }

    gpiod_chip_close(chip);
    return "key " + keyBuffer + ":";
}

// === Ana Uygulama ===
int main() {
    std::cout << "[DigitalIONode] Starting node..." << std::endl;

    SocketCon serverSocket;
    if (!serverSocket.init("0.0.0.0", 7002, true)) {
        std::cerr << "[DigitalIONode] Socket listen failed!" << std::endl;
        return -1;
    }

    std::cout << "[DigitalIONode] Waiting for connection...\n";
    SocketCon clientSocket = serverSocket.acceptClient();
    std::cout << "[DigitalIONode] Client connected.\n";

    Relay relay;
    relay.init();
    bool relayState = false;

    std::string message;
    while (clientSocket.receive(message)) {
        std::cout << "[DigitalIONode] Received: " << message << std::endl;

        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
        message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
        message.erase(std::remove(message.begin(), message.end(), ' '), message.end());

        if (message == "relay1:") {
            relay.set(true);
            relayState = true;
            clientSocket.send("relay ok:");
        } else if (message == "relay0:") {
            relay.set(false);
            relayState = false;
            clientSocket.send("relay ok:");
        } else if (message == "relayDurum:") {
            clientSocket.send("relay " + std::to_string(relayState) + ":");
        } else if (message == "sensorDurum:") {
            clientSocket.send("sensorDurum 0:");
        } else if (message == "sensorTip:") {
            clientSocket.send("sensorTip SICAKLIK:");
        } else if (message == "key:") {
            // sadece "key:" özelinde çoklu okuma
            std::string result = scanKeypadString();
            clientSocket.send(result);
        } else if (message == "keyOnce:") {
            // özel durumda sadece bir tuş döndür
            std::string result = scanKeypadOnce();
            clientSocket.send(result);
        } else if (message == "kapat:") {
            clientSocket.send("kapat ok:");
            break;
        } else {
            clientSocket.send("err: bilinmeyen komut");
        }
    }

    std::cout << "[DigitalIONode] Bağlantı sonlandı. Çıkılıyor.\n";

    relay.release();
    clientSocket.release();
    serverSocket.release();
    return 0;
}
