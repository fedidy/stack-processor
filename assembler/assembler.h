#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "../headers/processor.h"

typedef struct _assembler {
    file_info input_code;
    int* code;
    size_t size_code;
    size_t cap_code;
} assembler;

const int BASE_CODE_LENGTH = 10;
const int REALLOC_COEF = 2;

int AssemblerInit(assembler *ass);
int ParseText(assembler *ass);
int ParseCmd(assembler *ass);
int RecognizeCmd(const char cmd[], int *expected_arg);
int ParseArg(assembler *ass);
int ParseVarArg(assembler *ass);
int RecognizeVar(const char var_name[]);
int PushCode(assembler *ass, int elem);
int CodeReallocUp(assembler *ass);
int CodeRecalloc(assembler *ass);
void SkipLine(assembler *ass);
void CodeOutput(assembler *ass, const char *filename);
void AssDestructor(assembler *ass);

#endif
