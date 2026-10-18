#include "../headers/stack.h"

#include <stdio.h>
#include <assert.h>
#include <stdio.h>

Error_info StackVerify(Stack_t *const stk) {
#if STACK_DEBUG > 0
    CHECK_IF_STACK_NULL(stk)

    Error_info err = {};
    WRITE_ERR(err, NO_ERR)

    if (!stk->stack_name)
        WRITE_ERR(err, STRUCT_NAME_ERR)
    else if (!stk->creation_pos_info.file)
        WRITE_ERR(err, FILE_NAME_ERR)
    else if (!stk->creation_pos_info.func)
        WRITE_ERR(err, FUNC_NAME_ERR)
    else if (!stk->creation_pos_info.line)
        WRITE_ERR(err, LINE_NUM_ERR)
    else if (stk->capacity == 0 || stk->capacity > CAPACITY_MAX)
        WRITE_ERR(err, CAPACITY_ERR)
    else if (!stk->data)
        WRITE_ERR(err, NULL_POINTER_ERR);


    if (err.error_code) {
        PrintStackError(stk, err);
    }
    return err;
#else
    (void)stk;
    return 0;
#endif
}


Error_info PrintStackError(const Stack_t *const stk, const Error_info err) {
#if STACK_DEBUG > 0
    CHECK_IF_STACK_NULL(stk)

    fprintf(stderr, "Error in stack named: %s:\n", stk->stack_name);

    if (err.error_code) {
        fprintf(stderr, "GOT NO ERROR\n");
    }

    fprintf(stderr, "Error code = %d\n", err.error_code);

    if (stk->stack_name)
        fprintf(stderr, "Error in stack named %s\n", stk->stack_name);
    else
        fprintf(stderr, "NO STRUCTURE NAME\n");

    fprintf(stderr, "Pointer to structure = [%p]\n", stk);

    fprintf(stderr, "Error info:\n");
    if (err.pos.file)
        fprintf(stderr, "Filename: %s\n", err.pos.file);
    else
        fprintf(stderr, "NO FILE NAME\n");
    if (err.pos.func)
        fprintf(stderr, "Func: %s\n", err.pos.func);
    else
        fprintf(stderr, "NO FUNCTION NAME\n");
    if (err.pos.line)
        fprintf(stderr, "Line number = %d\n", err.pos.line);
    else
        fprintf(stderr, "NO LINE NUMBER\n");

    fprintf(stderr, "Data creation info:\n");
    if (stk->creation_pos_info.file)
        fprintf(stderr, "Filename: %s\n", stk->creation_pos_info.file);
    else
        fprintf(stderr, "NO FILE NAME\n");
    if (stk->creation_pos_info.func)
        fprintf(stderr, "Func: %s\n", stk->creation_pos_info.func);
    else
        fprintf(stderr, "NO FUNCTION NAME\n");
    if (stk->creation_pos_info.line)
        fprintf(stderr, "Line number = %d\n", stk->creation_pos_info.line);
    else
        fprintf(stderr, "NO LINE NUMBER\n");

    fprintf(stderr, "Stack capacity = %zu\n", stk->capacity);
    if (stk->capacity == 0)
        fprintf(stderr, "STACK CAPACITY IS ZERO\n");

    fprintf(stderr, "size = %zu\n", stk->size);

    DataPrint(stk);

    RETURN_NO_ERR
#else
    (void)stk;
    return 0;
#endif
}


/*уже не нужно по идее */
Error_info PrintStackLog(const Stack_t *const stk) {
#if STACK_DEBUG > 1
    CHECK_IF_STACK_NULL(stk)

    PRINT_LOG("======================Stack_t DUMP log %s======================\n", stk->stack_name);

    PRINT_LOG("Pointer to structure = [%p]\n", stk);

    PRINT_LOG("Data creation info:\n");
    PRINT_LOG("\tWhere: %s%d\n", stk->creation_pos_info.file, stk->creation_pos_info.line);
    PRINT_LOG("\tFunc: %s\n", stk->creation_pos_info.func);

    PRINT_LOG("Stack capacity = %zu\n", stk->capacity);
    PRINT_LOG("size = %zu\n", stk->size);

    LOG_DBG(
    DataPrint(stk);
    )

    PRINT_LOG("======================Ended DUMP log======================\n");

    RETURN_NO_ERR
#elif STACK_DEBUG > 0
    (void)stk;
    RETURN_NO_ERR;
#else
    (void)stk;
    return 0;
#endif
}
