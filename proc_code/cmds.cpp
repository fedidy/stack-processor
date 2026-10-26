#include "../headers/processor.h"
#include "../headers/cmds.h"

int CmdHLT(...) {
    return HLT_VAL;
}

int CmdPUSH(processor *proc) {
    proc->ip++;
    StackPush(proc->stack, proc->code[proc->ip]);
    proc->ip++;
    return 0;
}

int CmdPUSHR(processor *proc) {
    proc->ip++;
    StackPush(proc->stack, proc->reg[proc->code[proc->ip]]);
    proc->ip++;
    return 0;
}

int CmdPOP(processor *proc) {
    proc->ip++;
    StackPop(proc->stack, &proc->reg[proc->code[proc->ip]]);
    proc->ip++;
    return 0;
}

int CmdIN(processor *proc) {
    int a = 0;
    while (scanf("%d", &a) == 0) {printf("Wrong input try again\n");}
    StackPush(proc->stack, a);
    proc->ip++;
    return 0;
}

int CmdCALL(processor *proc) {
    StackPush(proc->func_st, (int) proc->ip + 2);
    proc->ip = (size_t) proc->code[proc->ip + 1];
    return 0;
}

int CmdRET(processor *proc) {
    int ret_address = -1;
    StackPop(proc->func_st, &ret_address);
    proc->ip = (size_t) ret_address;
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
    if (a2 == 0) {
        OUTPUT_ERROR("Division by zero\n");
        return -1;
    }
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
    printf("%d\n", a);
    proc->ip++;
    return 0;
}

int CmdJMP(processor *proc) {
    proc->ip = (size_t) proc->code[proc->ip + 1];
    return 0;
}

int CmdJA(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a > b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

int CmdJAE(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a >= b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

int CmdJB(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a < b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

int CmdJBE(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a <= b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

int CmdJE(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a == b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

int CmdJNE(processor *proc) {
    int a = 0, b = 0;
    StackPop(proc->stack, &a);
    StackPop(proc->stack, &b);
    if (a != b) {
        return CmdJMP(proc);
    }
    else {
        proc->ip++;
        proc->ip++;
        return 0;
    }
}

/*
int CmdJPORTKI(processor *proc) {

}
*/


int CmdDUMP(processor *proc) {
    StackDump(proc->stack);
    proc->ip++;
    return 0;
}


int CmdTOP(processor *proc) {
    int a = 0;
    StackTop(proc->stack, &a);
    printf("%d\n", a);
    proc->ip++;
    return 0;
}
