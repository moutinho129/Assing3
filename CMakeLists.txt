cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(assign3)

target_sources(app PRIVATE src/main.c src/button_task.c src/led_task.c src/task.c src/cmd.c)

target_include_directories(app PRIVATE include)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

