# 🔌 Modular Embedded Sensor & Actuator System

A modular, node-based C++ project designed for Raspberry Pi to control and monitor various hardware components including relays, sensors, and a keypad. The system follows a real **client-server architecture**, where each component is an independent node communicating via sockets.

---

## 🧠 Project Overview

This system simulates a **mini hardware automation network**, where:

- A **ClientNode** acts as the user interface.
- A central **ServerNode** routes incoming commands.
- Independent nodes manage:
  - **Relay & Keypad** (DigitalIONode)
  - **Gyroscope, Accelerometer & Temperature** (GyroSensorNode)

All communication is handled through **socket-based TCP/IP messaging**, making the system scalable, extendable and modular.

---

## ⚙️ System Architecture

```text
+-------------+         +-------------+         +-------------------+
| ClientNode  | <--->   | ServerNode  | <--->   | DigitalIONode     |
| (User Menu) |         |  (Router)   |         | (Relay / Keypad)  |
+-------------+         +-------------+         +-------------------+
                                          \
                                           \-->  GyroSensorNode
                                                (MPU6050 via I2C)

Each node runs independently and communicates with the ServerNode using specific port assignments.

🔧 Features
🔢 Menu-driven control via physical 4x4 keypad

📡 TCP socket communication between all nodes

⚙️ Relay control (on/off/status)

🌡️ Sensor monitoring: temperature, gyroscope, accelerometer

🎹 Keypad input (single press & multi-character entry)

🔁 Auto mode: relay reacts to sensor data

🧱 Modular node-based code structure (easy to expand)

###############################################################################################################################################################################################################################
Project Structure
.
├── ClientNode.cpp       # Menu system (runs on external device)
├── ServerNode.cpp       # Central hub, routes commands
├── DigitalIONode.cpp    # Controls relay + reads keypad
├── GyroSensorNode.cpp   # I2C communication with MPU6050
├── inc/                 # Header files
│   ├── RelayLib.h
│   ├── GyroLib.h
│   ├── KeypadLib.h
│   ├── SocketConLib.h
│   └── ...
├── src/                 # Source files
│   ├── RelayLib.cpp
│   ├── GyroLib.cpp
│   ├── SocketConLib.cpp
│   └── ...
├── CMakeLists.txt       # Build configuration
################################################################################################################################################################################################################################
🛠️ Setup & Run
🔌 Requirements
Raspberry Pi (with GPIO & I2C enabled)
MPU6050 Sensor
4x4 Matrix Keypad
Relay Module
libgpiod, wiringPi, CMake, g++ (C++17)

# On Raspberry Pi (in different terminals)
./ServerNode
./DigitalIONode
./GyroSensorNode

# On your laptop (or same Pi)
./ClientNode
