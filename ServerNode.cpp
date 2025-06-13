#include "../inc/SocketConLib.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "[ServerNode] Başlatılıyor...\n";

    SocketCon listenSocket;
    if (!listenSocket.init("0.0.0.0", 7001, true)) {
        std::cerr << "[ServerNode] Client için port başlatılamadı!\n";
        return -1;
    }

    std::cout << "[ServerNode] Client bağlantısı bekleniyor...\n";
    SocketCon server = listenSocket.acceptClient();
    std::cout << "[ServerNode] Client bağlantısı alındı.\n";


    SocketCon digitalIO;
    if (!digitalIO.init("127.0.0.1", 7002, false)) {
        std::cerr << "[ServerNode] DigitalIO Node'a bağlanılamadı!\n";
        return -1;
    }
    std::cout << "[ServerNode] DigitalIO bağlantısı başarılı.\n";

    SocketCon gyro;
    bool gyroConnected = gyro.init("127.0.0.1", 7003, false);
    if (!gyroConnected) {
        std::cerr << "[ServerNode] ⚠️ GyroSensor Node'a bağlanılamadı, devam ediliyor.\n";
    } else {
        std::cout << "[ServerNode] GyroSensor bağlantısı başarılı.\n";
    }

    std::cout << "[ServerNode] Tüm bağlantılar tamam. Client komut bekleniyor...\n";

    while (true) {
        std::string clientMsg;
        if (!server.receive(clientMsg)) {
            std::cerr << "[ServerNode] Client bağlantısı kesildi.\n";
            break;
        }

        std::cout << "[Client ➜ ServerNode] " << clientMsg << std::endl;
        std::string reply;

        if (clientMsg == "kapat:") {
            digitalIO.send("kapat:");
            std::string dRep;
            digitalIO.receive(dRep);

            std::string gRep = "kapat ok:";
            if (gyroConnected) {
                gyro.send("kapat:");
                gyro.receive(gRep);
            }

            reply = (dRep == "kapat ok:" && gRep == "kapat ok:") ? "kapat ok:" : "kapat err:";
        }

        else if (
            clientMsg.find("sensorDurum") == 0 ||
            clientMsg.find("relay") == 0 ||
            clientMsg.find("relayDurum") == 0 ||
            clientMsg.find("key") == 0 ||
            clientMsg.find("sensorTip") == 0
        ) {
            digitalIO.send(clientMsg);
            digitalIO.receive(reply);
        }

        else if (
            clientMsg.find("gyro") == 0 ||
            clientMsg.find("acc") == 0 ||
            clientMsg.find("temp") == 0
        ) {
            if (gyroConnected) {
                gyro.send(clientMsg);
                gyro.receive(reply);
            } else {
                reply = "gyro err:";
            }
        }

        else {
            reply = "err: geçersiz komut";
        }

        std::cout << "[ServerNode ➜ Client] " << reply << std::endl;
        server.send(reply);
    }

    // Temiz kapat
    server.release();
    digitalIO.release();
    gyro.release();
    return 0;
}
