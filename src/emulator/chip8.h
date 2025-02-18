#ifndef CHIP8_H
#define CHIP8_H

#include "core/data_types.h"

#define CHIP8_DISPLAY_SIZE 64*32
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32
#define CHIP8_DISPLAY_SCALE 16
#define CHIP8_FPS 60

void CHIP8_Init();
void CHIP8_Update();
void CHIP8_Cleanup();

void CHIP8_Render();
b8 CHIP8_SetPixel(i8 x, i8 y);
void CHIP8_Clear();

#endif