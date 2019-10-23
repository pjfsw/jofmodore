#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H

typedef struct {
    int opcode;
    char mnemonic[4];
    int addressingMode;
} Instruction;


Instruction *getInstruction(int opcode);

#endif /* _INSTRUCTIONS_H */