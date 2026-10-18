#include "../headers/processor.h"
#include "../headers/cmds.h"
#include <stdio.h>

int CmdHLT(...) {
    return 1;
}

int CmdPUSH(processor *proc) {
    proc->ip++;
    StackPush(proc->stack, proc->code[proc->ip]);
    proc->ip++;
    return 0;
}

int CmdADD(processor *proc) {
    int a1 = 0, a2 = 0;
    StackPop(proc->stack, &a1);
    StackPop(proc->stack, &a2);
    StackPush(proc->stack, a1 + a2);
    proc->ip++;
    return 0;
}

int CmdSUB(processor *proc) {
    int a1 = 0, a2 = 0;
    StackPop(proc->stack, &a1);
    StackPop(proc->stack, &a2);
    StackPush(proc->stack, a1 - a2);
    proc->ip++;
    return 0;
}

int CmdDIV(processor *proc) {
    int a1 = 0, a2 = 0;
    StackPop(proc->stack, &a1);
    StackPop(proc->stack, &a2);
    StackPush(proc->stack, a1 / a2);
    proc->ip++;
    return 0;
}

int CmdMULT(processor *proc) {
    int a1 = 0, a2 = 0;
    StackPop(proc->stack, &a1);
    StackPop(proc->stack, &a2);
    StackPush(proc->stack, a1 * a2);
    proc->ip++;
    return 0;
}

int CmdOUT(processor *proc) {
    int a = 0;
    StackPop(proc->stack, &a);
    printf(PRINT_DATA "\n", a);
    proc->ip++;
    return 0;
}

int CmdJMP(processor *proc) {
    proc->ip = (size_t) proc->code[proc->ip + 1];
    return 0;
}
