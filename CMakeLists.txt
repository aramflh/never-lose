cmake_minimum_required(VERSION 3.23)
project(never_lose C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(never_lose
        main.c
        utils.h keypad.h)
