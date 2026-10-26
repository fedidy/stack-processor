#include "assembler.h"

#include "../headers/files_func.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



int main(const int argc, const char* argv[]) {
    if (argc < 3) {
        OUTPUT_ERROR("No filename, bro\n");
        return -1;
    }

    Assemble(argv[1], argv[2]);

    return 0;
}


int Assemble(const char* input_file, const char* output_file) {
    assembler ass = {};

    if (AssemblerInit(&ass)) {abort();}

    if (FileCopy(&ass.input_code, input_file)) {abort();}

    ParseText(&ass); // первый проход
    AssemblerCleaner(&ass);
    ParseText(&ass); // второй проход

    CodeOutput(&ass, output_file);

    AssDestructor(&ass);

    return 0;
}


int AssemblerInit(assembler *ass) {
    assert(ass);

    ass->input_code.code = NULL;
    ass->input_code.length = 0;
    ass->input_code.ind = 0;

    ass->code = (int*) calloc(BASE_CODE_LENGTH, sizeof(int));
    if (ass->code == NULL) {
        OUTPUT_ERROR("Couldn't initialize ass->code\n");
        return -1;
    }
    ass->cap_code = BASE_CODE_LENGTH;
    ass->size_code = 0;

    for (int i = 0; i < LABELS_SIZE; i++) {
        ass->labels[i] = 0;
    }

    return 0;
}

void AssemblerCleaner(assembler *ass) {
    assert(ass);

    for (size_t i = 0; i < ass->cap_code; i++) {
        ass->code[i] = NO_DATA;
    }
    ass->size_code = 0;
    ass->input_code.ind = 0;
}

void AssDestructor(assembler *ass) {
    assert(ass);

    free(ass->code);
    ass->size_code = 0;
    ass->cap_code = 0;
    ass->input_code.ind = 0;
    free(ass->input_code.code);
    ass->input_code.length = 0;

    for (int i = 0; i < LABELS_SIZE; i++) {
        ass->labels[i] = 0;
    }
}


int ParseText(assembler *ass) {
    assert(ass);
    char symb = '\0';
    int expected_arg = 0;
    while (ass->input_code.ind < (size_t) ass->input_code.length) {
        symb = ass->input_code.code[ass->input_code.ind];
        if (expected_arg != 0) {
            if (isdigit(symb)) {
                //OUTPUT_ERROR("symb = %c, expected_arg = %d\n", symb, expected_arg);
                if (ParseArg(ass) < 0) {return -1;}
                expected_arg--;
            }
            else if (isalpha(symb)) {
                if (ass->code[ass->size_code - 1] == PUSH) {ass->code[ass->size_code - 1] = PUSHR;}
                if (ParseRegArg(ass) < 0) {return -1;}
                expected_arg--;
            }
            else if (symb == ' ') {
                ass->input_code.ind++;
            }
            else if (symb == ':') {
                UseLabel(ass);
                expected_arg--;
            }
            else if (symb == '[') {
                if (ass->code[ass->size_code - 1] == PUSH) {ass->code[ass->size_code - 1] = PUSHM;}
                if (ass->code[ass->size_code - 1] == POP) {ass->code[ass->size_code - 1] = POPM;}
                ParseMemory(ass);
                expected_arg--;
            }
            else {
                OUTPUT_ERROR("Got not expected symb (wanted argument): %c\n", symb);
                return -1;
            }
        }
        else {
            if (isalpha(symb)) {
                expected_arg = ParseCmd(ass);
            }
            else if (symb == '\0') {
                break;
            }
            else if (isdigit(symb)) {
                OUTPUT_ERROR("Got num argument when command expected: expected_arg = %d\n", expected_arg);
                return -1;
            }
            else if (symb == ';') {
                SkipLine(ass);
            }
            else if (isspace(symb)) {
                //if (expected_arg != 0) {OUTPUT_ERROR("Line ended when expected_arg = %d\n", __FILE__, __LINE__, expected_arg);}
                ass->input_code.ind++;
            }
            else if (symb == ':') {
                GetLabel(ass);
            }
            else {
                OUTPUT_ERROR("Got not expected symb: %c\n", symb);
                abort();
            }
        }
    }
    if (expected_arg != 0) {
        OUTPUT_ERROR("By the end of parsing expected_arg = %d\n", expected_arg);
        return -1;
    }
    return 0;
}

int ParseCmd(assembler *ass) {
    assert(ass);

    int expected_arg = 0;
    cmd_names cmd_num = NO_CMD;
    char cmd_name[MAX_CMD_LENGTH + 1] = "";
    int ind = 0;
    while (isalpha(ass->input_code.code[ass->input_code.ind])) {
        cmd_name[ind] = ass->input_code.code[ass->input_code.ind];
        ind++;
        ass->input_code.ind++;
    }
    if (RecognizeCmd(cmd_name, &expected_arg, &cmd_num) < 0) return -1;
    if (cmd_num < 0) return -1;
    if (PushCode(ass, cmd_num)) return -1;

    return expected_arg;
}

int RecognizeCmd(const char cmd[], int *expected_arg, cmd_names *cmd_num) {
    assert(expected_arg);
    assert(cmd_num);
    assert(cmd);


    #define COMPARE_CMD(CMD, EXP_CMD, EXP_ARG) \
    if (!strcmp(CMD, #EXP_CMD)) {*expected_arg = EXP_ARG; *cmd_num = EXP_CMD; return 0;}

    COMPARE_CMD(cmd, HLT, 0);
    COMPARE_CMD(cmd, PUSH, 1);
    COMPARE_CMD(cmd, PUSHR, 1);
    COMPARE_CMD(cmd, POP, 1);
    COMPARE_CMD(cmd, IN, 0);
    COMPARE_CMD(cmd, CALL, 1);
    COMPARE_CMD(cmd, RET, 0);
    COMPARE_CMD(cmd, ADD, 0);
    COMPARE_CMD(cmd, SUB, 0);
    COMPARE_CMD(cmd, DIV, 0);
    COMPARE_CMD(cmd, MULT, 0);
    COMPARE_CMD(cmd, OUT, 0);
    COMPARE_CMD(cmd, JMP, 1);
    COMPARE_CMD(cmd, JA, 1);
    COMPARE_CMD(cmd, JAE, 1);
    COMPARE_CMD(cmd, JB, 1);
    COMPARE_CMD(cmd, JBE, 1);
    COMPARE_CMD(cmd, JE, 1);
    COMPARE_CMD(cmd, JNE, 1);
    COMPARE_CMD(cmd, JPORTKI, 1);
    COMPARE_CMD(cmd, DUMP, 0);
    COMPARE_CMD(cmd, TOP, 0);

    #undef COMPARE_CMD

    OUTPUT_ERROR("Unknown command: \"%s\"\n", cmd);
    return -1;
}


int ParseArg(assembler *ass) {
    assert(ass);

    int arg = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.code + ass->input_code.ind, "%d%n", &arg, &symb_num)) {
        OUTPUT_ERROR("Can't scan arg\n");
        return -1;
    }
    if (symb_num < 0) {abort();}

    ass->input_code.ind += (unsigned) symb_num;
    return PushCode(ass, arg);
}


int ParseRegArg(assembler *ass) {
    assert(ass);

    char var_name[MAX_REG_LENGTH + 1] = "";
    int ind = 0;
    while (isalpha(ass->input_code.code[ass->input_code.ind])) {
        var_name[ind] = ass->input_code.code[ass->input_code.ind];
        ind++;
        ass->input_code.ind++;
    }
    int var = RecognizeVar(var_name);
    if (var < 0) return -1;
    return PushCode(ass, var);
}

int RecognizeVar(const char var_name[]) {
    assert(var_name);

    int var = var_name[0] - 'A';

    return var;
}


int PushCode(assembler *ass, int elem) {
    assert(ass);

    if (ass->size_code == ass->cap_code) {
        if (CodeReallocUp(ass)) return -1;
    }

    ass->code[ass->size_code++] = elem;

    return 0;
}


int CodeReallocUp(assembler *ass) {
    assert(ass);

    ass->cap_code = ass->cap_code * REALLOC_COEF;
    return CodeRecalloc(ass);

    return 0;
}

int CodeRecalloc(assembler *ass) {
    assert(ass);

    ass->code = (int*) realloc(ass->code, sizeof(int) * ass->cap_code);
    if (!ass->code) {
    OUTPUT_ERROR("Can't reallocate code %p, capacity = %zu, size = %zu\n",
                 ass->code, ass->cap_code, ass->size_code);
    return -1;
    }

    for (size_t i = ass->size_code; i < ass->cap_code; i++) {
        ass->code[i] = 0;
    }

    return 0;
}


void SkipLine(assembler *ass) {
    assert(ass);

    while(ass->input_code.code[ass->input_code.ind] != '\n') {
        ass->input_code.ind++;
    }

    ass->input_code.ind++;
}


int GetLabel(assembler *ass) {
    assert(ass);

    ass->input_code.ind++;

    int label_num = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.code + ass->input_code.ind,
                "%d%n", &label_num, &symb_num)) {
        OUTPUT_ERROR("Can't scan arg\n");
        return -1;
    }

    if (symb_num < 0) {abort();}

    ass->input_code.ind += (unsigned) symb_num;

    ass->labels[label_num] = (int) ass->size_code;

    ass->input_code.ind++;

    return 0;
}

int UseLabel(assembler *ass) {
    assert(ass);

    ass->input_code.ind++;

    int label_num = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.code + ass->input_code.ind,
                "%d%n", &label_num, &symb_num)) {
        OUTPUT_ERROR("Can't scan arg\n");
        return -1;
    }
    assert(symb_num >= 0);

    ass->input_code.ind += (unsigned) symb_num;

    PushCode(ass, ass->labels[label_num]);

    ass->input_code.ind++;

    return 0;
}


int ParseMemory(assembler *ass) {
    assert(ass);

    ass->input_code.ind++;

    int mem_num = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.code + ass->input_code.ind,
                "%d%n", &mem_num, &symb_num)) {
        OUTPUT_ERROR("Can't scan arg\n");
        return -1;
    }
    assert(symb_num >= 0);

    ass->input_code.ind += (unsigned) symb_num;
    if (ass->input_code.code[ass->input_code.ind] != ']') {
        OUTPUT_ERROR("Got %c instead of ']'\n", ass->input_code.code[ass->input_code.ind]);
    }
    ass->input_code.ind++;

    return PushCode(ass, mem_num);
}


int CodeOutput(assembler *ass, const char *filename) {
    assert(ass);

    FILE* fp = fopen(filename, "w");

    if (fp == NULL) {
        OUTPUT_ERROR("Can't open a file to output code\n");
        return -1;
    }

    #if BINARY > 0
        if (fwrite(ass->code, sizeof(int), ass->size_code, fp) != ass->size_code) {
            OUTPUT_ERROR("Not all code have been written\n");
        }
    #else
    for (size_t i = 0; i < ass->size_code; i++) {
        fprintf(fp, "%d ", ass->code[i]);
    }
    #endif

    fclose(fp);
    return 0;
}
