#include "../inc/SocketConLib.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>   // thread için
#include <vector>
#include <algorithm>

SocketCon::SocketCon() {
    sockfd = -1;
    isConnected = false;
    isServer = false;
}

SocketCon::~SocketCon() {
    release();
}

bool SocketCon::init(const std::string& ip, int port, bool asServer) {
    isServer = asServer;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "[SocketCon] Socket creation failed.\n";
        return false;
    }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (asServer) {
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "[SocketCon] Bind failed.\n";
            return false;
        }

        if (listen(sockfd, 5) < 0) {
            std::cerr << "[SocketCon] Listen failed.\n";
            return false;
        }

        std::cout << "[SocketCon] Listening on port " << port << "...\n";
    } else {
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
            std::cerr << "[SocketCon] Invalid IP address.\n";
            return false;
        }

        if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            std::cerr << "[SocketCon] Connection failed.\n";
            return false;
        }

        isConnected = true;
    }

    return true;
}

void SocketCon::release() {
    if (isConnected && sockfd >= 0) {
        close(sockfd);
        isConnected = false;
        std::cout << "[SocketCon] Connection closed.\n";
    }
}

bool SocketCon::send(const std::string& message) {
    if (!isConnected) return false;

    ssize_t sent = ::send(sockfd, message.c_str(), message.length(), 0);
    return sent == (ssize_t)message.length();
}

bool SocketCon::receive(std::string& message) {
    if (!isConnected) return false;

    char buffer[1024] = {0};
    ssize_t len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0) return false;

    message = std::string(buffer, len);
    return true;
}

SocketCon SocketCon::acceptClient() {
    SocketCon clientCon;
    sockaddr_in clientAddr {};
    socklen_t len = sizeof(clientAddr);

    int clientFd = accept(sockfd, (struct sockaddr*)&clientAddr, &len);
    if (clientFd < 0) {
        std::cerr << "[SocketCon] Accept failed.\n";
        return clientCon;
    }

    clientCon.sockfd = clientFd;
    clientCon.isConnected = true;
    clientCon.isServer = false;

    return clientCon;
}


