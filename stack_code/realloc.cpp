#include "../headers/stack.h"

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

Error_info StackReallocUp(Stack_t *const stk) {
    CHECK_IF_STACK_NULL(stk)

    stk->capacity = stk->capacity * CAPACITY_UP_COEF;
    PRINT_LOG("Data pointer before recalloc = %p\n", stk->data);
    Error_info err_info = StackRecalloc(stk);
    PRINT_LOG("Data pointer after recalloc = %p\n", stk->data);
    RETURN_ERROR_IF_GOT(err_info);
    ON_DBG(
    if (!stk->data) {
        RETURN_ERR(REALLOC_UP_ERR)
    })

    PRINT_LOG("STACK AFTER RECALLOC\n");
    LOG_DBG(StackDump(stk));

    return StackVerify(stk);
}

//rewrite
Error_info StackReallocDown(Stack_t *const stk) {
    CHECK_IF_STACK_NULL(stk)

    stk->capacity = (stk->capacity / CAPACITY_DOWN_COEF) + 1;
    stk->data = (stack_elem_t*) realloc(stk->data, sizeof(stack_elem_t) * stk->capacity);
    ON_DBG(
    if (!stk->data) {
        RETURN_ERR(REALLOC_DOWN_ERR)
    })

    return StackVerify(stk);
}


Error_info StackRecalloc(Stack_t *const stk) {
    CHECK_IF_STACK_NULL(stk)

    stk->data = (stack_elem_t*) realloc(stk->data, sizeof(stack_elem_t) * stk->capacity);
    if (!stk->data) {
        RETURN_ERR(DATA_RECALLOC_ERR);
    }

    for (size_t i = stk->size; i < stk->capacity; i++) {
        stk->data[i] = NO_DATA;
    }

    RETURN_NO_ERR;
}
