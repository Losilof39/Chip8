#include <stdio.h>
#include "engine/application/app.h"
#include "emulator/chip8.h"

int main(int argc, char** argv)
{
    static Application chip8;

    chip8.title = "Chip8 Emulator";
    chip8.version = "0.0.1";
    chip8.mbUsing = 1;
    chip8.width = CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_SCALE;
    chip8.height = CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_SCALE;
    chip8.Init = CHIP8_Init;
    chip8.Update = CHIP8_Update;
    chip8.Cleanup = CHIP8_Cleanup;

    Application_Run(&chip8);
    
    return 0;
}