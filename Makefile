TARGET = fishjit
PREFIX = /usr/local

LUA = luajit

DYNASM_DIR = luajit-2.0/dynasm
DYNASM = ${LUA} ${DYNASM_DIR}/dynasm.lua

CFLAGS += -MMD -std=c11 -D_DEFAULT_SOURCE -I${DYNASM_DIR} -Wall -Wextra -g
LDFLAGS +=
DYNASMFLAGS += -D X64

DSRC = fish-compiler.dasc
SRC = ${DSRC:.dasc=.c} \
	  fish-cache.c fish-codebox.c fish-runtime.c fish-runtime.c fish-stack.c
OBJ = ${SRC:.c=.o}

all: ${TARGET}

${TARGET}: ${OBJ}

%.c: %.dasc
	${DYNASM} ${DYNASMFLAGS} -o $@ $<

check: all
	./tests/run-tests.sh ./${TARGET} ./tests

install: all
	install -Dm755 ${TARGET} "${DESTDIR}${PREFIX}/bin"

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/${TARGET}"

clean:
	rm -rf *.d *.o ${DSRC:.dasc=.c} ${TARGET}

.PHONY: all check install uninstall clean

-include *.d
