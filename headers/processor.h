#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

#if STEP_BY_STEP > 0
    #define WAIT_INP() getchar()
#else
    #define WAIT_INP() (void) 0
#endif

#define OUTPUT_ERROR(text, ...)\
fprintf(stderr, "%s:%d:" text, __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

typedef enum _cmd_names {
    HLT,
    PUSH,
    PUSHR,
    PUSHM,
    POP,
    POPM,
    IN,
    CALL,
    RET,
    ADD, SUB, DIV, MULT,
    OUT,
    JMP,
    JA,
    JAE,
    JB ,
    JBE,
    JE,
    JNE,
    JPORTKI,
    DUMP,
    TOP,
    NO_CMD
} cmd_names;

typedef struct _file_info {
    off_t length;
    char* code;
    size_t ind;
} file_info;

const size_t REGISTER_COUNT = 26;
const size_t RAM_SIZE = 1024;
const int MAX_CMD_LENGTH = 10;
const int MAX_REG_LENGTH = 2;
typedef struct _processor {
    int *code;
    size_t code_length;
    size_t ip;
    Stack_t *stack;
    Stack_t *func_st;
    int reg[REGISTER_COUNT];
    int ram[RAM_SIZE];
} processor;

#endif
