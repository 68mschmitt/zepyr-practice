cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app_display_init)

target_sources(app PRIVATE src/main.c)
set(LVGL_DIR ${ZEPHYR_LVGL_MODULE_DIR})

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

target_include_directories(app PRIVATE
    ${LVGL_DIR}/demos/
)
