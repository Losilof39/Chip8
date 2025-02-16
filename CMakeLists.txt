cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Chip8 VERSION 1.0)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

option(LOG_COLORFUL "Logging messages will be in different colors" ON)

if(LOG_COLORFUL)
	add_definitions(-DLOG_USE_COLOR)
endif()

#add_definitions(-DASSET_PATH=\"${CMAKE_SOURCE_DIR}/assets\")
#add_definitions(-DSHADER_PATH=\"${CMAKE_SOURCE_DIR}/shaders\")

set(SOURCES
src/main.c
vendor/glad/src/glad.c)

add_executable(Chip8 ${SOURCES})

target_include_directories(Chip8 PRIVATE vendor/glad/include)