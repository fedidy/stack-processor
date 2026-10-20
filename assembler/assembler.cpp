#include "assembler.h"

#include "../headers/files_func.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>



int main(const int argc, const char* argv[]) {
    if (argc < 3)
        return -fprintf(stderr, "%s:%d: No filename, bro\n", __FILE__, __LINE__);

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

    ass->input_code.file_copy = NULL;
    ass->input_code.file_length = 0;
    ass->input_code.file_ind = 0;

    ass->code = (int*) calloc(BASE_CODE_LENGTH, sizeof(int));
    if (ass->code == NULL) {return -fprintf(stderr, "%s:%d: Couldn't initialize ass->code\n", __FILE__, __LINE__);}
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
    ass->input_code.file_ind = 0;
}

void AssDestructor(assembler *ass) {
    assert(ass);

    free(ass->code);
    ass->size_code = 0;
    ass->cap_code = 0;
    ass->input_code.file_ind = 0;
    free(ass->input_code.file_copy);
    ass->input_code.file_length = 0;

    for (int i = 0; i < LABELS_SIZE; i++) {
        ass->labels[i] = 0;
    }
}


int ParseText(assembler *ass) {
    assert(ass);
    char symb = '\0';
    int expected_arg = 0;
    while (ass->input_code.file_ind < (size_t) ass->input_code.file_length) {
        symb = ass->input_code.file_copy[ass->input_code.file_ind];
        if (expected_arg != 0) {
            if (isdigit(symb)) {
                //fprintf(stderr, "%s:%d: symb = %c, expected_arg = %d\n", __FILE__, __LINE__, symb, expected_arg);
                if (ParseArg(ass) < 0) {return -1;}
                expected_arg--;
            }
            else if (isalpha(symb)) {
                if (ass->code[ass->size_code - 1] == PUSH) {ass->code[ass->size_code - 1] = PUSH_R;}
                if (ParseVarArg(ass) < 0) {return -1;}
                expected_arg--;
            }
            else if (symb == ' ') {
                ass->input_code.file_ind++;
            }
            else if (symb == ':') {
                UseLabel(ass);
                expected_arg--;
            }
            else {
                fprintf(stderr, "%s:%d: Got not expected symb (wanted argument): %c\n", __FILE__, __LINE__, symb);
                abort();
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
                return -fprintf(stderr, "%s:%d: Got num argument when command expected: expected_arg = %d\n", __FILE__, __LINE__, expected_arg);
            }
            else if (symb == ';') {
                SkipLine(ass);
            }
            else if (isspace(symb)) {
                //if (expected_arg != 0) {return -fprintf(stderr, "%s:%d: Line ended when expected_arg = %d\n", __FILE__, __LINE__, expected_arg);}
                ass->input_code.file_ind++;
            }
            else if (symb == ':') {
                GetLabel(ass);
            }
            else {
                fprintf(stderr, "%s:%d: Got not expected symb: %c\n", __FILE__, __LINE__, symb);
                abort();
            }
        }
    }
    if (expected_arg != 0) return -fprintf(stderr, "%s:%d: By the end of parsing expected_arg = %d\n", __FILE__, __LINE__, expected_arg);
    return 0;
}

int ParseCmd(assembler *ass) {
    assert(ass);

    int expected_arg = 0;
    char cmd_name[MAX_CMD_LENGTH + 1] = "";
    int ind = 0;
    while (isalpha(ass->input_code.file_copy[ass->input_code.file_ind])) {
        cmd_name[ind] = ass->input_code.file_copy[ass->input_code.file_ind];
        ind++;
        ass->input_code.file_ind++;
    }
    int cmd = RecognizeCmd(cmd_name, &expected_arg);
    if (cmd < 0) return -1;
    if (PushCode(ass, cmd)) return -1;

    return expected_arg;
}

int RecognizeCmd(const char cmd[], int *expected_arg) {
    assert(expected_arg);
    assert(cmd);

    if (!strcmp(cmd, "HLT")) {return HLT;}
    if (!strcmp(cmd, "PUSH")) {*expected_arg = 1; return PUSH;}
    if (!strcmp(cmd, "PUSHr")) {*expected_arg = 1; return PUSH_R;}
    if (!strcmp(cmd, "POP")) {*expected_arg = 1; return POP;}
    if (!strcmp(cmd, "ADD")) {return ADD;}
    if (!strcmp(cmd, "SUB")) {return SUB;}
    if (!strcmp(cmd, "DIV")) {return DIV;}
    if (!strcmp(cmd, "MULT")) {return MULT;}
    if (!strcmp(cmd, "OUT")) {return OUT;}
    if (!strcmp(cmd, "JMP")) {*expected_arg = 1; return JMP;}
    if (!strcmp(cmd, "JA")) {*expected_arg = 1; return JA;}
    if (!strcmp(cmd, "JAE")) {*expected_arg = 1; return JAE;}
    if (!strcmp(cmd, "JB")) {*expected_arg = 1; return JB;}
    if (!strcmp(cmd, "JBE")) {*expected_arg = 1; return JBE;}
    if (!strcmp(cmd, "JE")) {*expected_arg = 1; return JE;}
    if (!strcmp(cmd, "JNE")) {*expected_arg = 1; return JNE;}
    if (!strcmp(cmd, "JPORTKI")) {*expected_arg = 1; return JPORTKI;}
    if (!strcmp(cmd, "DUMP")) {return DUMP;}
    if (!strcmp(cmd, "TOP")) {return TOP;}
    else return -fprintf(stderr, "%s:%d: Unknown command: \"%s\"\n", __FILE__, __LINE__, cmd);
}


int ParseArg(assembler *ass) {
    assert(ass);

    int arg = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.file_copy + ass->input_code.file_ind, "%d%n", &arg, &symb_num))
        return -fprintf(stderr, "%s:%d: Can't scan arg\n", __FILE__, __LINE__);
    if (symb_num < 0) {abort();}

    ass->input_code.file_ind += (unsigned) symb_num;
    return PushCode(ass, arg);
}


int ParseVarArg(assembler *ass) {
    assert(ass);

    char var_name[MAX_VAR_LENGTH + 1] = "";
    int ind = 0;
    while (isalpha(ass->input_code.file_copy[ass->input_code.file_ind])) {
        var_name[ind] = ass->input_code.file_copy[ass->input_code.file_ind];
        ind++;
        ass->input_code.file_ind++;
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
    if (!ass->code) return -fprintf(stderr, "%s:%d: Can't reallocate code %p, capacity = %zu, size = %zu\n",
                                            __FILE__, __LINE__, ass->code, ass->cap_code, ass->size_code);

    for (size_t i = ass->size_code; i < ass->cap_code; i++) {
        ass->code[i] = 0;
    }

    return 0;
}


void SkipLine(assembler *ass) {
    assert(ass);

    while(ass->input_code.file_copy[ass->input_code.file_ind] != '\n') {
        ass->input_code.file_ind++;
    }

    ass->input_code.file_ind++;
}


int GetLabel(assembler *ass) {
    assert(ass);

    ass->input_code.file_ind++;

    int flag_num = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.file_copy + ass->input_code.file_ind, "%d%n", &flag_num, &symb_num))
        return -fprintf(stderr, "%s:%d: Can't scan arg\n", __FILE__, __LINE__);
    if (symb_num < 0) {abort();}

    ass->input_code.file_ind += (unsigned) symb_num;

    ass->labels[flag_num] = (int) ass->size_code;

    ass->input_code.file_ind++;

    return 0;
}

int UseLabel(assembler *ass) {
    assert(ass);

    ass->input_code.file_ind++;

    int flag_num = 0;
    int symb_num = 0;
    if (!sscanf(ass->input_code.file_copy + ass->input_code.file_ind, "%d%n", &flag_num, &symb_num))
        return -fprintf(stderr, "%s:%d: Can't scan arg\n", __FILE__, __LINE__);
    if (symb_num < 0) {abort();}

    ass->input_code.file_ind += (unsigned) symb_num;

    PushCode(ass, ass->labels[flag_num]);

    ass->input_code.file_ind++;

    return 0;
}


void CodeOutput(assembler *ass, const char *filename) {
    assert(ass);

    FILE* fp = fopen(filename, "w");

    for (size_t i = 0; i < ass->size_code; i++) {
        fprintf(fp, "%d ", ass->code[i]);
    }
}
