TARGET = fishjit
PREFIX = /usr/local

CC ?= cc
LUA ?= luajit

DYNASM_DIR = luajit-2.0/dynasm
DYNASM = ${LUA} ${DYNASM_DIR}/dynasm.lua

CFLAGS += -std=c11 -D_DEFAULT_SOURCE -I${DYNASM_DIR} -Wall -Wextra -flto
LDFLAGS += -flto
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

check: all
	./test.sh ./${TARGET} ./tests

install: all
	install -Dm755 ${TARGET} "${DESTDIR}${PREFIX}/bin"

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/${TARGET}"

clean:
	rm -rf *.d *.o ${DSRC:.dasc=.c} ${TARGET}

.PHONY: all check install uninstall clean

-include *.d
