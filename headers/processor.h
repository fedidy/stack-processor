#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

/*
#define STEP_BY_STEP
*/

 #define BINARY 0

enum _cmds {
    HLT = 0,
    PUSH = 1,
    ADD = 2,
    SUB = 3,
    DIV = 4,
    MULT = 5,
    OUT = 6,
    JMP = 7,
    JA = 8,
    JAE = 9,
    JB = 10,
    JE = 11,
    JNE = 12,
    JPORTKI = 13,
    STDUMP = 14,
    TOP = 15
};

typedef struct _file_info {
    off_t file_length;
    char* file_copy;
    size_t file_ind;
} file_info;

typedef struct _processor {
    int *code;
    size_t code_length;
    size_t ip;
    Stack_t *stack;
} processor;

const int MAX_CMD_LENGTH = 4;


#endif
