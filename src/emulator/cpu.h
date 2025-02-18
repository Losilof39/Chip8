#ifndef CPU_H
#define CPU_H

#include "core/data_types.h"

void CPU_Init();
void CPU_Cleanup();
void CPU_LoadSpriteToMemory();
void CPU_LoadProgramToMemory();
void CPU_Cycle();
void CPU_ExecuteInstruction(u16 opcode);
void CPU_UpdateTimers();
void CPU_PlaySound();

#endif