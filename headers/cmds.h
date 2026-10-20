#ifndef CMDS_H
#define CMDS_H

#include "processor.h"

int CmdHLT(...);
int CmdPUSH(processor *proc);
int CmdPUSHr(processor *proc);
int CmdPOP(processor *proc);
int CmdADD(processor *proc);
int CmdSUB(processor *proc);
int CmdDIV(processor *proc);
int CmdMULT(processor *proc);
int CmdOUT(processor *proc);
int CmdJMP(processor *proc);
int CmdJA(processor *proc);
int CmdJAE(processor *proc);
int CmdJB(processor *proc);
int CmdJBE(processor *proc);
int CmdJE(processor *proc);
int CmdJNE(processor *proc);
int CmdDUMP(processor *proc);
int CmdTOP(processor *proc);

#endif
