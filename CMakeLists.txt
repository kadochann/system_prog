cmake_minimum_required(VERSION 3.10)
project(RCS_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Header dosyaları için inc klasörünü tanıt
include_directories(inc)

# Alt dizin (src/) içindeki CMake'i çağır
add_subdirectory(src)

# Ana Node Executable'lar (src dışındalar!)
add_executable(ClientNode ClientNode.cpp src/KeypadLib.cpp)
add_executable(ServerNode ServerNode.cpp)
add_executable(GyroSensorNode GyroSensorNode.cpp)
add_executable(DigitalIONode DigitalIONode.cpp src/RelayLib.cpp)

# Kütüphaneleri linkle (alt CMake'de oluşturulacak)
target_link_libraries(ClientNode SocketConLib pthread gpiod)
target_link_libraries(ServerNode SocketConLib pthread)
target_link_libraries(GyroSensorNode GyroLib SocketConLib wiringPi pthread)
target_link_libraries(DigitalIONode SocketConLib wiringPi gpiod pthread)

