cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(module_tests)

target_include_directories(app PRIVATE
    src
)

target_sources(app PRIVATE 
    src/main.cpp
    src/scd40.cpp
    src/hV_Board.cpp
    src/hV_Colours565.cpp
    src/hV_Font_Terminal.cpp
    src/hV_Screen_Buffer.cpp
    src/hV_Utilities_Common.cpp
    src/hV_Utilities_PDLS.cpp
    src/Screen_EPD_EXT3.cpp
)