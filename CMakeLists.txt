cmake_minimum_required(VERSION 4.0)
project(wonderhoycxx)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(DPP REQUIRED)
find_package(Skia REQUIRED)

add_executable(wonderhoycxx
        src/main.cpp
        src/bot.cpp
        src/commands/command_handler.cpp
        src/commands/create.cpp
        src/modules/create_sticker.cpp
        src/data/characters.cpp
        src/commands/characters.cpp
        src/utils/parse_emoji.cpp
        src/utils/parse_hex_color.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${DPP_LIBRARIES}
        Skia::Skia
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${DPP_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/src
)