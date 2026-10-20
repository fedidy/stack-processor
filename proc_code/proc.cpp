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

    for (int i = 0; i < REGISTER_SIZE; i++) {
        proc->reg[i] = 0;
    }
}

void ProcessorDtor(processor *proc) {
    assert(proc);

    free(proc->code);
    proc->code_length = 0;
    proc->ip = 0;
    StackDtor(proc->stack);

    for (int i = 0; i < REGISTER_SIZE; i++) {
        proc->reg[i] = 0;
    }
}


int ParseCode(const char *filename, processor *proc) {
    assert(filename); assert(proc);

    proc->code_length = (size_t) MeasureFile(filename);
    proc->code = (int*) calloc((size_t) proc->code_length, sizeof(int));

    FILE* code_file = fopen(filename, "r");
    if (code_file == NULL) return -fprintf(stderr, "%s:%d Can't open file: %s\n", __FILE__, __LINE__, filename);

#if BINARY == 0
    int cmd = 0;
    size_t ind = 0;
    while (fscanf(code_file, "%d ", &cmd) != EOF) {
        //printf("ind = %zu, cmd = %d\n", ind, cmd);
        proc->code[ind] = cmd;
        ind++;
        //cmd = -1;
    }
#else

#endif
    fclose(code_file);
    return 0;
}


int RunCode(processor *proc) {
    assert(proc);

    while (proc->ip < proc->code_length) {
        if (RunCommand(proc) == HLT_VAL) {
            return 0;
        }
        WAIT_INP
    }
    fprintf(stderr, "Code ended w/o HLT\n");
    return -1;
}

int RunCommand(processor *proc) {
    assert(proc);
    PRINT_LOG("Running command %d, with ip = %zu\n", proc->code[proc->ip], proc->ip);
    switch (proc->code[proc->ip]) {
        case HLT:
            return CmdHLT(proc);
        case PUSH:
            return CmdPUSH(proc);
        case PUSH_R:
            return CmdPUSHr(proc);
        case POP:
            return CmdPOP(proc);
        case ADD:
            return CmdADD(proc);
        case SUB:
            return CmdSUB(proc);
        case DIV:
            return CmdDIV(proc);
        case MULT:
            return CmdMULT(proc);
        case OUT:
            return CmdOUT(proc);
        case JMP:
            return CmdJMP(proc);
        case JA:
            return CmdJA(proc);
        case JAE:
            return CmdJAE(proc);
        case JB:
            return CmdJB(proc);
        case JBE:
            return CmdJBE(proc);
        case JE:
            return CmdJE(proc);
        case JNE:
            return CmdJNE(proc);
        case DUMP:
            return CmdDUMP(proc);
        case TOP:
            return CmdTOP(proc);
        default:
            fprintf(stderr, "%s:%d: Got unknown command", __FILE__, __LINE__);
            abort();
    }
}
