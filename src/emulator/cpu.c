#include <stdlib.h>
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
static u8 delayTimer = 0;
static u8 soundTimer = 0;

// Program counter. Stores the currently executing address.
static u16 pc = 0x200;

// Don't initialize this with a size in order to avoid empty results.
static u16 *stack;

// Some instructions require pausing, such as Fx0A.
static b8 paused = false;
static u8 speed = 10;

void CPU_Init()
{
    srand(time(NULL));

    memory = (u8*)Z_Malloc(4096 * sizeof(u8), PU_STATIC, NULL);

    CPU_LoadSpriteToMemory();
    CPU_LoadProgramToMemory("ROM/BLITZ");
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
        {
            switch (opcode) {
                case 0x00E0:
                {
                    CHIP8_Clear();
                }break;

                case 0x00EE:
                {
                    //pc = stack.pop();
                }break;
            }
    
        }break;

        case 0x1000:
        {
            pc = (opcode & 0xFFF);
        }break;

        case 0x2000:
        {
            //stack.push(pc);
            pc = (opcode & 0xFFF);
        }break;

        case 0x3000:
        {
            if (v[x] == (opcode & 0xFF))
                pc += 2;
        }break;

        case 0x4000:
        {
            if (v[x] != (opcode & 0xFF))
                pc += 2;
        }break;

        case 0x5000:
        {
            if (v[x] == v[y]) 
                pc += 2;
            
        }break;

        case 0x6000:
        {
            v[x] = (opcode & 0xFF);
        }break;

        case 0x7000:
        {
            v[x] += (opcode & 0xFF);
        }break;

        case 0x8000:
        {
            switch (opcode & 0xF) {
                case 0x0:
                {
                    v[x] = v[y];
                }break;

                case 0x1:
                {
                    v[x] |= v[y];
                }break;

                case 0x2:
                {
                    v[x] &= v[y];
                }break;

                case 0x3:
                {
                    v[x] ^= v[y];
                }break;

                case 0x4:
                {
                    i16 sum = (v[x] += v[y]);

                    v[0xF] = 0;

                    if (sum > 0xFF)
                        v[0xF] = 1;

                    v[x] = sum;
                }break;

                case 0x5:
                {
                    v[0xF] = 0;

                    if (v[x] > v[y])
                        v[0xF] = 1;

                    v[x] -= v[y];
                }break;

                case 0x6:
                {
                    v[0xF] = (v[x] & 0x1);
                    v[x] >>= 1;
                }break;

                case 0x7:
                {
                    v[0xF] = 0;

                    if (v[y] > v[x])
                        v[0xF] = 1;

                    v[x] = v[y] - v[x];
                }break;

                case 0xE:
                {
                    v[0xF] = (v[x] & 0x80);
                    v[x] <<= 1;
                }break;
            }

        }break;

        case 0x9000:
        {
            if (v[x] != v[y])
                pc += 2;
        }break;

        case 0xA000:
        {
            i = (opcode & 0xFFF);
        }break;
        
        case 0xB000:
        {
            pc = (opcode & 0xFFF) + v[0];
        }break;

        case 0xC000:
        {
            u8 random = (u8)floor( (float)rand() / (float)RAND_MAX * 0xFF);

            v[x] = random & (opcode & 0xFF);
        }break;

        case 0xD000:
        {
            u8 width = 8;
            u8 height = (opcode & 0xF);

            v[0xF] = 0;

            for (u8 row = 0; row < height; row++) 
            {
                u8 sprite = memory[i + row];

                for (u8 col = 0; col < width; col++) 
                {
                    // If the bit (sprite) is not 0, render/erase the pixel
                    if ((sprite & 0x80) > 0) 
                    {
                        // If setPixel returns 1, which means a pixel was erased, set VF to 1
                        if (CHIP8_SetPixel(v[x] + col, v[y] + row))
                            v[0xF] = 1;
                    }

                    // Shift the sprite left 1. This will move the next next col/bit of the sprite into the first position.
                    // Ex. 10010000 << 1 will become 0010000
                    sprite <<= 1;
                }
            }
        }break;

        case 0xE000:
        {
            switch (opcode & 0xFF) {
                case 0x9E:
                {
                    //if (keyboard.isKeyPressed(this.v[x]))
                    //    pc += 2;
                }break;

                case 0xA1:
                {
                    //if (!keyboard.isKeyPressed(this.v[x])) {
                    //    pc += 2;
                }break;
            }
    
        }break;

        case 0xF000:
        {
            switch (opcode & 0xFF) {
                case 0x07:
                {
                    v[x] = delayTimer;
                }break;

                case 0x0A:
                {
                    paused = true;
                }break;

                case 0x15:
                {
                    delayTimer = v[x];
                }break;

                case 0x18:
                {
                    soundTimer = v[x];
                }break;

                case 0x1E:
                {
                    i += v[x];
                }break;

                case 0x29:
                {
                    i = v[x] * 5;
                }break;

                case 0x33:
                {
                    memory[i] = (u8)(v[x] / 100);

                    memory[i + 1] = (u8)((v[x] % 100) / 10);
                
                    memory[i + 2] = (u8)(v[x] % 10);

                }break;

                case 0x55:
                {
                    for (u8 registerIndex = 0; registerIndex <= x; registerIndex++) {
                        memory[i + registerIndex] = v[registerIndex];
                    }
                }break;

                case 0x65:
                {
                    for (u8 registerIndex = 0; registerIndex <= x; registerIndex++) {
                        v[registerIndex] = memory[i + registerIndex];
                    }
                }break;
            }
    
        }break;
    
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