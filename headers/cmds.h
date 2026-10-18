#ifndef CMDS_H
#define CMDS_H

#include "processor.h"

int CmdHLT(...);
int CmdPUSH(struct _processor *proc);
int CmdADD(struct _processor *proc);
int CmdSUB(struct _processor *proc);
int CmdDIV(struct _processor *proc);
int CmdMULT(struct _processor *proc);
int CmdOUT(struct _processor *proc);
int CmdJMP(struct _processor *proc);


#endif
