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

add_definitions(-DASSET_PATH=\"${CMAKE_SOURCE_DIR}/assets\")
add_definitions(-DSHADER_PATH=\"${CMAKE_SOURCE_DIR}/src/engine/core/renderer/shaders\")

set(SOURCES
src/main.c
src/engine/application/app.c
src/engine/application/app.h
src/engine/core/audio.c
src/engine/core/audio.h
src/engine/core/filesystem.c
src/engine/core/filesystem.h
src/engine/core/common.c
src/engine/core/common.h
src/engine/core/data_types.h
src/engine/core/core.h
src/engine/core/input.c
src/engine/core/input.h
src/engine/core/log.c
src/engine/core/log.h
src/engine/core/time.c
src/engine/core/time.h
src/engine/core/zone.c
src/engine/core/zone.h
src/engine/core/renderer/font.c
src/engine/core/renderer/font.h
src/engine/core/renderer/r_shader.c
src/engine/core/renderer/r_shader.h
src/engine/core/renderer/renderer2D.c
src/engine/core/renderer/renderer2D.h
vendor/glad/src/glad.c)

add_executable(Chip8 ${SOURCES})

target_include_directories(Chip8 PRIVATE src/engine)
target_include_directories(Chip8 PRIVATE vendor/glad/include)