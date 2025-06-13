#include "../inc/SocketConLib.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void printMenu() {
    cout << "\n🧭 Menü:\n";
    cout << "1. Sensör durumunu göster\n";
    cout << "2. Röleyi kontrol et (Aç/Kapat)\n";
    cout << "3. Röle durumu\n";
    cout << "4. Keypad oku\n";
    cout << "5. Gyro verileri (gyro/temp/acc)\n";
    cout << "6. Otomatik denetim modu\n";
    cout << "0. Kapat\n";
    cout << "Seçiminizi keypad ile yapın : ";
}

string getKeypadOnce(SocketCon& socket) {
    socket.send("keyOnce:");
    string resp;
    socket.receive(resp);

    if (resp.find("key ") == 0 && resp.size() >= 6) {
        return string(1, resp[4]);
    }
    return "x";
}

string getKeypadString(SocketCon& socket) {
    socket.send("key:");
    string resp;
    socket.receive(resp);
    if (resp.find("key ") == 0) {
        return resp.substr(4, resp.size() - 5); // 'key ...:'
    }
    return "";
}

int main() {
    SocketCon socket;
    if (!socket.init("127.0.0.1", 7001, false)) {
        cerr << "[ClientNode] ❌ Server bağlantısı başarısız!\n";
        return -1;
    }

    while (true) {
        printMenu();
        string choice = getKeypadOnce(socket);
        cout << "Seçim: " << choice << endl;

        if (choice == "0") {
            socket.send("kapat:");
            string resp;
            socket.receive(resp);
            cout << resp << endl;
            break;
        }

        else if (choice == "1") {
            socket.send("sensorDurum:");
            string response;
            socket.receive(response);
            cout << "➤ " << response << endl;
        }

        else if (choice == "2") {
            cout << "➤ Röle durumu için tuşa basın [0=Kapat, 1=Aç]: ";  // 🔹 önce ekrana yaz
            this_thread::sleep_for(chrono::milliseconds(500));           // 🔹 kullanıcı görsün diye kısa bekleme
            string val = getKeypadOnce(socket);                         // 🔹 sonra input al
            cout << "Tuş: " << val << endl;

            if (val == "0" || val == "1") {
                socket.send("relay " + val + ":");
                string response;
                socket.receive(response);
                cout << "➤ " << response << endl;
            } else {
                cout << "❌ Geçersiz röle seçimi.\n";
            }
        }

        else if (choice == "3") {
            socket.send("relayDurum:");
            string response;
            socket.receive(response);
            cout << "➤ " << response << endl;
        }

        else if (choice == "4") {
            cout << "[Keypad] Tuşlara basın, '#' ile tamamlayın:\n";
            string keyseq = getKeypadString(socket);
            cout << "KEY: " << keyseq << endl;
        }

        else if (choice == "5") {
            for (const string& cmd : {"temp:", "gyro:", "acc:"}) {
                socket.send(cmd);
                string response;
                socket.receive(response);
                cout << "➤ " << response << endl;
            }
        }

        else if (choice == "6") {
            socket.send("sensorTip:");
            string tip;
            socket.receive(tip);
            cout << "Sensör Tipi: " << tip << endl;

            string trigger = "";
            while (trigger != "0" && trigger != "1") {
                cout << "Hangi durumda röle ON olsun? [0/1]: ";  // 🔹 önce ekrana yaz
                this_thread::sleep_for(chrono::milliseconds(500));  // 🔹 kısa bekleme
                trigger = getKeypadOnce(socket);                  // 🔹 sonra input al
                cout << "Tuş: " << trigger << endl;

                if (trigger != "0" && trigger != "1") {
                    cout << "❌ Geçersiz giriş. Lütfen sadece 0 veya 1 tuşlayın.\n";
                }
            }

            cout << "🔁 Otomatik denetim başladı. Çıkmak için '#' tuşlayın...\n";

            while (true) {
                socket.send("sensorDurum:");
                string status;
                socket.receive(status);
                cout << "➤ " << status << endl;

                if (status.find("sensorDurum " + trigger + ":") != string::npos) {
                    socket.send("relay 1:");
                } else {
                    socket.send("relay 0:");
                }

                string r;
                socket.receive(r);
                this_thread::sleep_for(chrono::seconds(2));

                cout << "(Çıkmak için '#' tuşlayın): ";
                string exitCheck = getKeypadOnce(socket);
                if (exitCheck == "#") break;
            }
        }

        else {
            cout << "❌ Geçersiz seçim.\n";
        }
    }

    socket.release();
    return 0;
}
