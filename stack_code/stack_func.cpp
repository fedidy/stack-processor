#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../headers/stack.h"

Error_info StackCtor(Stack_t *const stk, const size_t capacity, const char *const stack_name) {
    PRINT_LOG("Started StackCtor\n");
    CHECK_IF_STACK_NULL(stk)

    ON_DBG(
    if (!stack_name) {
        RETURN_ERR(STRUCT_NAME_ERR);
    }
    stk->stack_name = stack_name;
    stk->creation_pos_info.file = __FILE__;
    stk->creation_pos_info.func = __func__;
    stk->creation_pos_info.line = __LINE__;
    )

    stk->capacity = capacity;
    stk->data = (stack_elem_t*) calloc(capacity, sizeof(stack_elem_t));
    if (!stk->data) {
        RETURN_ERR(CALLOC_ERR);
    }
    for (size_t i = 0; i < capacity; i++) {
        stk->data[i] = NO_DATA;
    }
    stk->size = 0;
    PRINT_LOG("Ended StackCtor\n");
    RETURN_NO_ERR;
}


Error_info StackPushArray(Stack_t *const stk, const stack_elem_t *const data, const size_t count) {
    PRINT_LOG("Started StackPushArray\n");
    CHECK_IF_STACK_NULL(stk)

    StackVerify(stk);

    PrintStackLog(stk);

    if (!data) {
        RETURN_ERR(NULL_POINTER_ERR);
    }

    Error_info err_info = {};
    WRITE_ERR(err_info, NO_ERR)
    for (size_t i = 0; i < count; i++) {
        PRINT_LOG("Before pushing element = " PRINT_DATA "\n", data[i]);
        PRINT_LOG("err_code = %d, i = %lu\n", err_info.error_code, i);

        err_info = StackPush(stk, data[i]);

        //RETURN_ERROR_IF_GOT(err_info)
    }

    StackVerify(stk);

    RETURN_NO_ERR;
}

Error_info StackPush(Stack_t *const stk, const stack_elem_t elem) {
    CHECK_IF_STACK_NULL(stk)

    Error_info err_info = {};
    if (stk->size == stk->capacity) {
        err_info = StackReallocUp(stk);
        PRINT_LOG("err_code after ReallocUp: %d\n", err_info.error_code);
        PRINT_LOG("Capacity after ReallocUp = %zu\n", stk->capacity);
        RETURN_ERROR_IF_GOT(err_info);
    }

    PRINT_LOG("Pushing elem = " PRINT_DATA "\n", elem);

    stk->data[stk->size] = elem;

    stk->size++;

    PRINT_LOG("End of push:size = %zu, capacity = %zu\n", stk->size, stk->capacity);

    PRINT_LOG("STACK AFTER PUSHING ELEMENT\n");

    RETURN_NO_ERR;
}


Error_info StackPop(Stack_t *const stk, stack_elem_t *const elem) {
    CHECK_IF_STACK_NULL(stk)

    PRINT_LOG("Poping elem\n");

    if (stk->size == 0) {
        fprintf(stderr, "%s:%d: Tried StackPop from stack with zero elements\n", __FILE__, __LINE__);
        RETURN_ERR(POPING_FROM_ZERO_SIZE);
    }

    *elem = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = NO_DATA;
    stk->size--;

    PRINT_LOG("Poped elem = " PRINT_DATA "\n", *elem);


    if (stk->size == (stk->capacity / CAPACITY_DOWN_COEF) - 1) {
        return StackReallocDown(stk);
    }

    RETURN_NO_ERR;
}


Error_info StackTop(const Stack_t *const stk, stack_elem_t *const elem) {
    CHECK_IF_STACK_NULL(stk);
    if (elem == NULL) {
        RETURN_ERR(NULL_POINTER_ERR)
    }

    *elem = stk->data[stk->size - 1];

    RETURN_NO_ERR;
}


Error_info StackDtor(Stack_t *const stk) {
    CHECK_IF_STACK_NULL(stk)

    ON_DBG(
    stk->creation_pos_info.file = 0;
    stk->creation_pos_info.func = 0;
    stk->creation_pos_info.line = 0;
    stk->stack_name = 0;
    )
    free(stk->data);
    stk->size = 0;
    stk->capacity = 0;

    RETURN_NO_ERR;
}


void StackDump(const Stack_t *const stk) {
    if (stk == NULL) {
        fprintf(stderr, "Got zero pointer to stack while dumping\n");
        return;
    }

    #if STACK_DEBUG > 0
    fprintf(stderr, "======================Stack_t DUMP %s======================\n", stk->stack_name);
    #else
    fprintf(stderr, "======================Stack_t DUMP======================\n");
    #endif

    fprintf(stderr, "Pointer to structure = [%p]\n", stk);

    ON_DBG(
    fprintf(stderr, "Data creation info:\n");
    fprintf(stderr, "\tWhere: %s:%d\n", stk->creation_pos_info.file, stk->creation_pos_info.line);
    fprintf(stderr, "\tFunc: %s\n", stk->creation_pos_info.func);
    )

    fprintf(stderr, "Stack capacity = %zu\n", stk->capacity);
    fprintf(stderr, "size = %zu\n", stk->size);

    DataPrint(stk);

    fprintf(stderr, "======================Ended DUMP======================\n");
}


void DataPrint(const Stack_t *const stk) {
    ON_DBG(
    if (stk == NULL) {
        fprintf(stderr, "Got zero pointer to stack while printing\n");
        return;
    }
    if (stk->capacity == 0 || stk->capacity > CAPACITY_MAX) {
        fprintf(stderr, "Wrong capacity while printing\n");
        return;
    }
    )

    size_t i = 0;
    for (;i < stk->size; i++) {
        fprintf(stderr, "(*) data[%zu] = " PRINT_DATA "\n", i, stk->data[i]);
    }
    for (;i < stk->capacity; i++) {
        fprintf(stderr, "data[%zu] = " PRINT_DATA "\n", i, stk->data[i]);
    }
}
