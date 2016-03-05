#ifndef FISH_COMPILER_H
#define FISH_COMPILER_H

#include "dasm_proto.h"

#include "fish-stack.h"

struct fish_code
{
    /* compiled code takes a state argument, and sets it to the next state to
     * be executed when the code has finished
     *
     * returns 0 for success and non-zero otherwise (e.g. stack underflow) */
    int (*entry)(struct fish_state *,
                 struct fish_stack *,
                 struct fish_codebox *);

    /* buffer holding the machine code, created by mmap */
    size_t size;
    void *buffer;

    /* the maximum items this code can add to the stack, used to grow stack
     * before executing code */
    int max_stack_change;
};

/* compile a line of execution in codebox starting at state, returning a
 * malloc'ed fish_code structure */
struct fish_code *fish_compile(struct fish_codebox *codebox,
                               struct fish_state state);

/* free a fish_code struct */
void fish_free_code(struct fish_code *code);

#endif /* FISH_COMPILER_H */
