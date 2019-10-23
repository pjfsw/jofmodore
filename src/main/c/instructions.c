#include "instructions.h"
#include <stdlib.h>

Instruction instructions[] = {
    { 0x4C, "JMP", 0 },
    { 0xEA, "NOP", 0 },
};
int instructionCount = sizeof(instructions)/sizeof(instructions[0]);

Instruction *getInstruction(int opcode) {
    for (int i = 0; i < instructionCount; i++) {
        if (instructions[i].opcode == opcode) {
            return &instructions[i];
        }
    }
    return NULL;
}
