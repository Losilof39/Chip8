#include "cpu.h"
#include "core/core.h"

 // 4KB (4096 bytes) of memory
static u8 *memory;
static u8 *program;
static u32 sizeProgram;

// 16 8-bit registers
static u8 v[16];

// Stores memory addresses. Set this to 0 since we aren't storing anything at initialization.
static u16 i;

// Timers
static u16 delayTimer = 0;
static u16 soundTimer = 0;

// Program counter. Stores the currently executing address.
static u16 pc = 0x200;

// Don't initialize this with a size in order to avoid empty results.
static u16 *stack;

// Some instructions require pausing, such as Fx0A.
static b8 paused = false;
static u8 speed = 10;

void CPU_Init()
{
    memory = (u8*)Z_Malloc(4096 * sizeof(u8), PU_STATIC, NULL);
}

void CPU_Cleanup()
{
    Z_Free(memory);
}

void CPU_LoadSpriteToMemory()
{
    // Array of hex values for each sprite. Each sprite is 5 bytes.
    // The technical reference provides us with each one of these values.
    const u32 sprites[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    u32 length = sizeof(sprites);

    // According to the technical reference, sprites are stored in the interpreter section of memory starting at hex 0x000
    for (u16 i = 0; i < length; i++) {
        memory[i] = sprites[i];
    }
}

void CPU_LoadProgramToMemory()
{
    for (u32 loc = 0; loc < sizeProgram; loc++) {
        memory[0x200 + loc] = program[loc];
    }
}
