#ifndef PROC_H
#define PROC_H

#include "processor.h"

void ProcessorInit(processor *proc);
void ProcessorDtor(processor *proc);
int ParseCode(const char *filename, processor *proc);
int RunCode(processor *proc);
int RunCommand(processor *proc);

#endif
