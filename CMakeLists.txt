cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Chip8 VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE INTERNAL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)  # Forces shared libs
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)  # Avoids Brotli issues

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

option(LOG_COLORFUL "Logging messages will be in different colors" ON)

if(LOG_COLORFUL)
    add_definitions(-DLOG_USE_COLOR)
endif()

add_definitions(-DASSET_PATH=\"${CMAKE_SOURCE_DIR}/assets\")
add_definitions(-DSHADER_PATH=\"${CMAKE_SOURCE_DIR}/src/engine/core/renderer/shaders\")

# Add libraries without forcing rebuilds
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_mixer EXCLUDE_FROM_ALL)
add_subdirectory(vendor/cglm EXCLUDE_FROM_ALL)
add_subdirectory(vendor/freetype EXCLUDE_FROM_ALL)

# Prevent unnecessary rebuilding of static libraries
set_target_properties(freetype PROPERTIES EXCLUDE_FROM_ALL TRUE)
set_target_properties(cglm PROPERTIES EXCLUDE_FROM_ALL TRUE)

set(SOURCES
src/main.c
src/emulator/chip8.h
src/emulator/chip8.c
src/emulator/cpu.h
src/emulator/cpu.c
src/engine/core/data_types.h
src/engine/application/app.h
src/engine/application/app.c
src/engine/core/audio.h
src/engine/core/audio.c
src/engine/core/filesystem.h
src/engine/core/filesystem.c
src/engine/core/common.h
src/engine/core/common.c
src/engine/core/core.h
src/engine/core/input.h
src/engine/core/input.c
src/engine/core/log.h
src/engine/core/log.c
src/engine/core/time.h
src/engine/core/time.c
src/engine/core/zone.h
src/engine/core/zone.c
src/engine/core/renderer/font.h
src/engine/core/renderer/font.c
src/engine/core/renderer/r_shader.h
src/engine/core/renderer/r_shader.c
src/engine/core/renderer/renderer2D.h
src/engine/core/renderer/renderer2D.c
vendor/glad/src/glad.c)

add_executable(Chip8 ${SOURCES})

# Link only prebuilt versions of libraries to avoid unnecessary rebuilds
set(FREETYPE_LIB ${CMAKE_BINARY_DIR}/libfreetyped.dll)
set(CGLM_LIB ${CMAKE_BINARY_DIR}/libcglm-0.dll)
set(SDL_LIB ${CMAKE_BINARY_DIR}/SDL3.dll)
set(SDL_MIXER_LIB ${CMAKE_BINARY_DIR}/SDL3_mixer.dll)

#target_link_libraries(Chip8 PRIVATE cglm)
#target_link_libraries(Chip8 PRIVATE SDL3-shared)
#target_link_libraries(Chip8 PRIVATE SDL3_mixer-shared)
#target_link_libraries(Chip8 PRIVATE freetype)

target_include_directories(Chip8 PRIVATE src/engine)
target_include_directories(Chip8 PRIVATE vendor/glad/include)
target_include_directories(Chip8 PRIVATE vendor/SDL/include)
target_include_directories(Chip8 PRIVATE vendor/SDL_mixer/include)
target_include_directories(Chip8 PRIVATE vendor/cglm/include/cglm)
target_include_directories(Chip8 PRIVATE vendor/freetype/include)

target_link_libraries(Chip8 PRIVATE ${FREETYPE_LIB} ${CGLM_LIB} ${SDL_LIB} ${SDL_MIXER_LIB})