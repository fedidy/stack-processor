#ifndef FILES_FUNC_H
#define FILES_FUNC_H

#include "processor.h"

off_t MeasureFile(const char* filename);
int FileCopy(file_info *file_inf, const char *filename);

#endif
