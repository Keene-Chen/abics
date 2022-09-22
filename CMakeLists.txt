cmake_minimum_required(VERSION 3.0.0)

project(
  abics
  VERSION 0.1.0
  LANGUAGES C)

include_directories(${PROJECT_SOURCE_DIR}/include/camera)
include_directories(${PROJECT_SOURCE_DIR}/include/libmqttclient)
include_directories(${PROJECT_SOURCE_DIR}/include/serial_port)
find_library(LIB_MQTTCLIENT libmqttclient.so /usr/local/lib)
find_library(LIB_PTHREAD libpthread.so.0 /usr/lib/x86_64-linux-gnu)

set(SRC src/abics.c include/camera/camera.c include/serial_port/serial_port.c)

add_executable(abics ${SRC})
target_link_libraries(abics ${LIB_MQTTCLIENT} ${LIB_PTHREAD})