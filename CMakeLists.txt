cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(assign3)

# Lista de ficheiros da aplicação
FILE(GLOB app_sources
    src/main.c
    src/button_task.c
    src/led_task.c
    src/task.c
    src/control_task.c
    src/temp_task.c
    src/uart.c
    src/cmd.c


)

target_sources(app PRIVATE ${app_sources})

