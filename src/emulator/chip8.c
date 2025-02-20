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
    f32 delta = Time_GetDeltaTime();
    CPU_Cycle();

    const f32 frameTime = 1000.0f / 60.0f;  // 16.67ms per frame
    f32 elapsedMs = delta * 1000.0f;  // Convert delta to milliseconds

    if (elapsedMs < frameTime)
    {
        SDL_Delay((u32)(frameTime - elapsedMs));  // Wait for the next frame
    }
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

    R2D_DrawColoredQuad((vec3){x * CHIP8_DISPLAY_SCALE, y * CHIP8_DISPLAY_SCALE, 0}, (vec2){CHIP8_DISPLAY_SCALE, CHIP8_DISPLAY_SCALE}, (vec3){1.0f, 1.0f, 1.0f});

    return !display[pixelPos];
}

void CHIP8_Clear()
{
    memset(display, 0, CHIP8_DISPLAY_SIZE);
}