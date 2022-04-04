# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources}
src/Data_processing/data_processor.c;
src/Handlers/initiater.c;
src/Handlers/kalman_filter.c;
src/Handlers/observer.c;
src/Search_algorithms/search.c;
src/Servo/encoder.c;
src/Servo/servo.c
)

zephyr_library_include_directories(
    ${ZEPHYR_BASE}/samples/bluetooth
    src/Data_processing;
    src/Handlers;
    src/Search_algorithms;
    src/Servo)



