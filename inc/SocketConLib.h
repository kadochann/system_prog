#pragma once
#include <string>

class SocketCon {
private:
    int sockfd;
    bool isServer;
    bool isConnected;

public:
    SocketCon();
    ~SocketCon();
    SocketCon acceptClient(); // Server için client bağlantısını kabul et


    bool init(const std::string& ip, int port, bool asServer);  // server veya client olarak başlat
    void release();                                              // bağlantıyı kapat
    bool send(const std::string& message);                       // veri gönder
    bool receive(std::string& message);                          // veri al
};
