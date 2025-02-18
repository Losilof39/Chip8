#include "chip8.h"
#include "cpu.h"
#include "core/core.h"

// backbuffer
u8* display;

void CHIP8_Init()
{
    display = (u8*)Z_Malloc(CHIP8_DISPLAY_SIZE, PU_STATIC, NULL);
    memset(display, 0, CHIP8_DISPLAY_SIZE);

    CPU_Init();
}

void CHIP8_Update()
{

}

void CHIP8_Cleanup()
{
    CPU_Cleanup();
    
    Z_Free(display);
}

void CHIP8_Render()
{
}

b8 CHIP8_SetPixel(i8 x, i8 y)
{
    u8 pixelPos;

    if (x > CHIP8_DISPLAY_WIDTH) {
        x -= CHIP8_DISPLAY_WIDTH;
    } else if (x < 0) {
        x += CHIP8_DISPLAY_WIDTH;
    }
    
    if (y > CHIP8_DISPLAY_HEIGHT) {
        y -= CHIP8_DISPLAY_HEIGHT;
    } else if (y < 0) {
        y += CHIP8_DISPLAY_HEIGHT;
    }

    pixelPos = x + y * CHIP8_DISPLAY_WIDTH;

    display[pixelPos] ^= 1;

    return !display[pixelPos];
}

void CHIP8_Clear()
{
    memset(display, 0, CHIP8_DISPLAY_SIZE);
}