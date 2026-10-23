#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

#if STEP_BY_STEP > 0
    #define WAIT_INP getchar();
#else
    #define WAIT_INP
#endif


enum _cmds {
    HLT = 0,
    PUSH = 1,
    PUSHR = 2,
    POP = 3,
    IN = 4,
    CALL = 5,
    RET = 6,
    ADD = 8,
    SUB,
    DIV,
    MULT,
    OUT = 16,
    JMP = 32,
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

#define RUN_CMD(cmd, func, proc) case cmd: return func(proc);

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
    Stack_t *func_st;
} processor;

const int MAX_CMD_LENGTH = 10;
const int MAX_VAR_LENGTH = 2;
const int HLT_VAL = 6767;


#endif
