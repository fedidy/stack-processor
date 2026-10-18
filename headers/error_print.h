#ifndef ERROR_PRINT
#define ERROR_PRINT

#include "stack_defines.h"
Error_info StackVerify(Stack_t *const stk);
Error_info PrintStackError(const Stack_t *const stk, const Error_info err);
Error_info PrintStackLog(const Stack_t *const stk);

#endif
