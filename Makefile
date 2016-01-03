TARGET = fishjit
PREFIX = /usr/local

CC ?= cc
LUA ?= luajit

DYNASM_DIR = luajit-2.0/dynasm
DYNASM = ${LUA} ${DYNASM_DIR}/dynasm.lua

CFLAGS += -std=gnu11 -Wall -g -I${DYNASM_DIR}
LDFLAGS +=
DYNASMFLAGS += -D X64

DSRC = fish-compiler.dasc
SRC = ${DSRC:.dasc=.c} \
	fish-cache.c fish-codebox.c fish-runtime.c fish-runtime.c fish-stack.c \
	main.c
OBJ = ${SRC:.c=.o}

all: ${TARGET}

${TARGET}: ${OBJ}
	${CC} ${LDFLAGS} -o $@ $^

.c.o: $(DEPS)
	${CC} -MMD ${CFLAGS} -c -o $@ $<

%.c: %.dasc
	${DYNASM} ${DYNASMFLAGS} -o $@ $<

check: all test.sh
	./test.sh ./${TARGET} ./tests

install: all
	install -Dm755 ${TARGET} "${DESTDIR}${PREFIX}/bin"

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/${TARGET}"

clean:
	rm -rf *.d *.o ${DSRC:.dasc=.c} ${TARGET}

.PHONY: all check install uninstall clean

-include *.d
