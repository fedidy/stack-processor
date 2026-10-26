#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../headers/proc.h"
#include "../headers/cmds.h"
#include "../headers/files_func.h"



int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "%s:%d No filename\n", __FILE__, __LINE__);
        abort();
    }

    processor proc = {};
    ProcessorInit(&proc);

    ParseCode(argv[1], &proc);

    RunCode(&proc);

    ProcessorDtor(&proc);

    return 0;
}


void ProcessorInit(processor *proc) {
    assert(proc);

    proc->code = NULL;
    proc->code_length = 0;
    proc->ip = 0;
    proc->stack = (Stack_t*) malloc(sizeof(Stack_t));
    StackCtor(proc->stack, BASE_LENGTH, "stack");
    proc->func_st = (Stack_t*) malloc(sizeof(Stack_t));
    StackCtor(proc->func_st, BASE_LENGTH, "func_st");

    for (int i = 0; i < REGISTER_COUNT; i++) {
        proc->reg[i] = 0;
    }
}

void ProcessorDtor(processor *proc) {
    assert(proc);

    free(proc->code);
    proc->code_length = 0;
    proc->ip = 0;
    StackDtor(proc->stack);

    for (int i = 0; i < REGISTER_COUNT; i++) {
        proc->reg[i] = 0;
    }
}


int ParseCode(const char *filename, processor *proc) {
    assert(filename); assert(proc);

    proc->code_length = (size_t) MeasureFile(filename);
    proc->code = (int*) calloc((size_t) proc->code_length, sizeof(int));

    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        OUTPUT_ERROR("Can't open file: %s\n", filename);
        return -1;
    }

#if BINARY > 0
    if (fread(proc->code, sizeof(int), proc->code_length / sizeof(int), input_file)
        != proc->code_length / sizeof(int)) {
        return -fprintf(stderr, "%s:%d: Can't read all code\n", __FILE__, __LINE__);
    }
#else
    int cmd = 0;
    size_t ind = 0;
    while (fscanf(input_file, "%d ", &cmd) != EOF) {
        proc->code[ind] = cmd;
        ind++;
    }
#endif
    fclose(input_file);
    return 0;
}


int RunCode(processor *proc) {
    assert(proc);

    while (proc->ip < proc->code_length) {
        if (RunCommand(proc) == HLT_VAL) {
            return 0;
        }
        WAIT_INP();
    }
    fprintf(stderr, "Code ended w/o HLT\n");
    return -1;
}

int RunCommand(processor *proc) {
    assert(proc);
    PRINT_LOG("Running command %d, with ip = %zu\n", proc->code[proc->ip], proc->ip);
    #define RUN_CMD(cmd, func, proc) case cmd: return func(proc);
    switch (proc->code[proc->ip]) {
        RUN_CMD(HLT, CmdHLT, proc)
        RUN_CMD(PUSH, CmdPUSH, proc)
        RUN_CMD(PUSHR, CmdPUSHR, proc)
        RUN_CMD(POP, CmdPOP, proc)
        RUN_CMD(IN, CmdIN, proc)
        RUN_CMD(CALL, CmdCALL, proc)
        RUN_CMD(RET, CmdRET, proc)
        RUN_CMD(ADD, CmdADD, proc)
        RUN_CMD(SUB, CmdSUB, proc)
        RUN_CMD(DIV, CmdDIV, proc)
        RUN_CMD(MULT, CmdMULT, proc)
        RUN_CMD(OUT, CmdOUT, proc)
        RUN_CMD(JMP, CmdJMP, proc)
        RUN_CMD(JA, CmdJA, proc)
        RUN_CMD(JAE, CmdJAE, proc)
        RUN_CMD(JB, CmdJB, proc)
        RUN_CMD(JBE, CmdJBE, proc)
        RUN_CMD(JE, CmdJE, proc)
        RUN_CMD(JNE, CmdJNE, proc)
        RUN_CMD(DUMP, CmdDUMP, proc)
        RUN_CMD(TOP, CmdTOP, proc)
        default:
            fprintf(stderr, "%s:%d: Got unknown command: %d", __FILE__, __LINE__, proc->code[proc->ip]);
            abort();
    }
    #undef RUN_CMD
}
