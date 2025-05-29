cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app_gauge_v1)

target_sources(app PRIVATE src/main.c)
set(LVGL_DIR ${ZEPHYR_LVGL_MODULE_DIR})

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
