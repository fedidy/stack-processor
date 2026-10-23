CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align\
-Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral\
-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked\
-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2\
-Wsuggest-override -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new\
-fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla\
-Iheaders

SANITIZER = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CC = g++

STACK_CODE = stack_code/realloc.cpp \
       		 stack_code/stack_func.cpp \
       		 stack_code/error_print.cpp

ASSEMBLER_CODE = assembler/assembler.cpp

PROC_CODE = proc_code/proc.cpp \
			proc_code/cmds.cpp

SHARED_CODE = shared_code/files_func.cpp

DEBUG_MODE ?= debug

ifeq ($(DEBUG_MODE), debug)
	CFLAGS += ${SANITIZER} -D STACK_DEBUG=1 -O0
else ifeq ($(DEBUG_MODE), log)
	CFLAGS += ${SANITIZER} -D STACK_DEBUG=2
else ifeq ($(DEBUG_MODE), release)
	CFLAGS += -D NDEBUG -D STACK_DEBUG=0 -O2
endif


MODE ?= proc

BINARY ?= 1

STEP ?= 0

ifeq ($(MODE), proc)
	CODE = ${PROC_CODE}
	NAME = proc
else ifeq ($(MODE), assembler)
	CODE = ${ASSEMBLER_CODE}
	NAME = assembler
endif

CFLAGS += -D BINARY=${BINARY} -D STEP_BY_STEP=${STEP}

${NAME}.out: ${CODE} ${STACK_CODE} ${SHARED_CODE}
	@${CC} ${CFLAGS} $^ -o $@
