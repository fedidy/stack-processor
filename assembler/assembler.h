#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include "../headers/processor.h"

const int BASE_CODE_LENGTH = 10;
const int REALLOC_COEF = 2;
const int LABELS_SIZE = 32;

typedef struct _assembler {
    file_info input_code;
    int* code;
    size_t size_code;
    size_t cap_code;
    int labels[LABELS_SIZE];
} assembler;


int Assemble(const char* input_file, const char* output_file);
int AssemblerInit(assembler *ass);
void AssemblerCleaner(assembler *ass);
void AssDestructor(assembler *ass);
int ParseText(assembler *ass);
int ParseCmd(assembler *ass);
int RecognizeCmd(const char cmd[], int *expected_arg, cmd_names *cmd_num);
int ParseArg(assembler *ass);
int ParseVarArg(assembler *ass);
int RecognizeVar(const char var_name[]);
int PushCode(assembler *ass, int elem);
int CodeReallocUp(assembler *ass);
int CodeRecalloc(assembler *ass);
void SkipLine(assembler *ass);
int GetLabel(assembler *ass);
int UseLabel(assembler *ass);
int CodeOutput(assembler *ass, const char *filename);

#endif
