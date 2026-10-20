#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

/*
#define STEP_BY_STEP
*/

enum _cmds {
    HLT = 0,
    PUSH = 1,
    PUSH_R = 2,
    POP = 3,
    ADD = 4,
    SUB = 5,
    DIV = 6,
    MULT = 7,
    OUT = 8,
    JMP = 16,
    JA,
    JAE,
    JB ,
    JBE,
    JE,
    JNE,
    JPORTKI,
    DUMP = 42,
    TOP = 67
};

typedef struct _file_info {
    off_t file_length;
    char* file_copy;
    size_t file_ind;
} file_info;

const int REGISTER_SIZE = 26;
typedef struct _processor {
    int *code;
    size_t code_length;
    size_t ip;
    Stack_t *stack;
    int reg[REGISTER_SIZE];
} processor;

const int MAX_CMD_LENGTH = 10;
const int MAX_VAR_LENGTH = 2;
const int HLT_VAL = 6767;


#endif
