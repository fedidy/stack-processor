#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "stack.h"

#if STEP_BY_STEP > 0
    #define WAIT_INP() getchar()
#else
    #define WAIT_INP() (void) 0
#endif


typedef enum _cmd_names {
    HLT = 0,
    PUSH,
    PUSHR,
    POP,
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

#define OUTPUT_ERROR(text, ...)\
fprintf(stderr, "%s:%d:" text, __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

typedef struct _file_info {
    off_t file_length;
    char* file_copy;
    size_t file_ind;
} file_info;

const int REGISTER_COUNT = 26;
typedef struct _processor {
    int *code;
    size_t code_length;
    size_t ip;
    Stack_t *stack;
    int reg[REGISTER_COUNT];
    Stack_t *func_st;
} processor;

const int MAX_CMD_LENGTH = 10;
const int MAX_REG_LENGTH = 2;


#endif
