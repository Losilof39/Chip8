#include "cpu.h"
#include "chip8.h"
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

    CPU_LoadSpriteToMemory();
    CPU_LoadProgramToMemory("ROM/BLINKY");
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

void CPU_LoadProgramToMemory(const char* romName)
{
    FILE* romFile;
    const char* pathToAsset = Filesystem_GetAssetPath();
    char* romPath = FormatText("%s/%s", pathToAsset, romName);

    romFile = fopen(romPath, "rb");
    if (romFile == NULL) {
        log_error("Failed to read ROM: %s\n", romPath);
    }
    fseek(romFile, 0L, SEEK_END);
    sizeProgram = ftell(romFile) + 1;
    fclose(romFile);

    /* Read File for Content */
    romFile = fopen(romPath, "r");
    program = memset(Z_Malloc(sizeProgram, PU_STATIC, NULL), '\0', sizeProgram);
    fread(program, 1, sizeProgram - 1, romFile);
    fclose(romFile);

    for (u32 loc = 0; loc < sizeProgram; loc++) {
        memory[0x200 + loc] = program[loc];
    }
}

void CPU_ExecuteInstruction(u16 opcode)
{
    pc += 2;

    u8 x = (opcode & 0x0F00) >> 8;

    u8 y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    break;
                case 0x00EE:
                    break;
            }
    
            break;
        case 0x1000:
            break;
        case 0x2000:
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            switch (opcode & 0xF) {
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
                case 0xE:
                    break;
            }
    
            break;
        case 0x9000:
            break;
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            switch (opcode & 0xFF) {
                case 0x9E:
                    break;
                case 0xA1:
                    break;
            }
    
            break;
        case 0xF000:
            switch (opcode & 0xFF) {
                case 0x07:
                    break;
                case 0x0A:
                    break;
                case 0x15:
                    break;
                case 0x18:
                    break;
                case 0x1E:
                    break;
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
                    break;
            }
    
            break;
    
        default:
            log_error("Unknown opcode %04x", opcode);
    }
}

void CPU_UpdateTimers()
{
    if (delayTimer > 0) {
        delayTimer -= 1;
    }

    if (soundTimer > 0) {
        soundTimer -= 1;
    }
}

void CPU_PlaySound()
{
    if (soundTimer > 0) {
        // play since wave at 440
    } else {
        // stop playing
    }
}

void CPU_Cycle()
{
    for (u16 i = 0; i < speed; i++) {
        if (!paused) {
            u16 opcode = (memory[pc] << 8 | memory[pc + 1]);
            CPU_ExecuteInstruction(opcode);
        }
    }

    if (!paused) {
        CPU_UpdateTimers();
    }

    CPU_PlaySound();
    CHIP8_Render();
}