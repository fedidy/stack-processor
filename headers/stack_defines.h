#ifndef STACK_DEFINES_H
#define STACK_DEFINES_H

#if STACK_DEBUG > 0
#define ON_DBG(...) __VA_ARGS__
#else
#define ON_DBG(...)
typedef int Error_info;
#endif

#if STACK_DEBUG > 1
#define LOG_DBG(...) __VA_ARGS__
#define PRINT_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define LOG_DBG(...)
#define PRINT_LOG(...)
#endif

#if STACK_DEBUG > 0
struct Position_info {
    const char* file;
    const char* func;
    int line;
};
#define GET_POSITION(position) { \
    (position).file = __FILE__;      \
    (position).func = __func__;      \
    (position).line = __LINE__;      \
}


struct Error_info {
    Position_info pos;
    int error_code;
    //const char* comment;
};
#define WRITE_ERR(err_struct, err_code) {   \
    GET_POSITION((err_struct).pos);             \
    (err_struct).error_code = (err_code);       \
}
#define RETURN_ERR(err_code) {              \
    Error_info err_info_do_not_use = {};        \
    WRITE_ERR(err_info_do_not_use, (err_code))  \
    return err_info_do_not_use;                 \
}
#define RETURN_NO_ERR {\
    Error_info err_info_do_not_use = {};        \
    WRITE_ERR(err_info_do_not_use, NO_ERR)      \
    return err_info_do_not_use;                 \
}
#define RETURN_ERROR_IF_GOT(err_info) {     \
    if ((err_info).error_code) {               \
        return (err_info);                      \
    }                                           \
}
#define GET_ERR(err) (err)->error_code
#define CHECK_IF_STACK_NULL(stk) {          \
    if ((stk) == NULL) {                        \
        fprintf(stderr, "%s:%d: Got zero pointer to stack\n", __FILE__, __LINE__);\
        RETURN_ERR(NULL_POINTER_ERR)      \
    }                                           \
}
#define SAFE_RUN(func, err_info) {              \
    if ((err_inf) = (func)) {                   \
        ;                  \
    }                                           \
}
#else
#define GET_POSITION(position) {}
#define WRITE_ERR(err_struct, err_code) {err_struct = -1;}
#define RETURN_ERR(err_code) {return (err_code);}
#define RETURN_NO_ERR {return 0;}
#define RETURN_ERROR_IF_GOT(err_code) {\
if ((err_code)) {\
    return (err_code);\
}\
}
#define GET_ERR(err) (err)
#define CHECK_IF_STACK_NULL(stk) {}
#define RUN_FUNC(func, err) {(func)}
#endif


enum Stack_Errors {
    NO_ERR = 0,
    NULL_POINTER_ERR,
    CALLOC_ERR,
    REALLOC_UP_ERR,
    REALLOC_DOWN_ERR,
    DATA_RECALLOC_ERR,
    PUSH_ERR,
    STRUCT_NAME_ERR,
    FILE_NAME_ERR,
    FUNC_NAME_ERR,
    LINE_NUM_ERR,
    CAPACITY_ERR,
    POPING_FROM_ZERO_SIZE
};

enum Other_Error {
    FILE_OPEN_ERR = 1000,
    TEST_FILENAME_ERR,
    INSUFFICIENT_ELEM_ERR
};

#include "stack_type.h"
#include <stdio.h>

struct Stack_t {
    ON_DBG(
    Position_info creation_pos_info;
    const char* stack_name;
    )
    stack_elem_t* data;
    size_t size;
    size_t capacity;
};

const size_t CAPACITY_MAX = (size_t) -1000;

#endif
