# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources}
src/custom_files/kalman_filter.c;
src/custom_files/observer.c;
src/custom_files/button_leds.c)

zephyr_library_include_directories(
    ${ZEPHYR_BASE}/samples/bluetooth
    src/custom_files)

