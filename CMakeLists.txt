cmake_minimum_required(VERSION 3.20)
project(Multikino_projekt)

set(CMAKE_CXX_STANDARD 20)

add_executable(Multikino_projekt
        app_main.cpp
        data.cpp
        utils.cpp
        storage.cpp
        menus.cpp
        qr.cpp
        qrcodegen.c)
