#ifndef FISH_STACK_H
#define FISH_STACK_H

#include <stdlib.h>
#include <stdbool.h>

#include "fish-codebox.h"

#define FISH_STACK_ITEM_SIZE (9)

/*
 * The fish stack data consists of alternating numbers and type identifiers.
 * The numbers are either 32/64-bit integers (depending on architecture), or
 * doubles. The type identifier is located on top of the number on the stack,
 * so it can be read easily when popping. A stack item is always 8 bytes, even
 * on i386.
 */

/* constants identifying the type of stack items */
enum fish_type {
    INTEGER = 0,
    FLOAT = 1
};

/* fish interpreter stack (and register) */
struct fish_stack
{
    size_t num_items;
    size_t max_items;
    fish_number *data;

    /* register holds a single item */
    uint8_t register_set;
    uint8_t register_type;
    uint64_t register_value; /* not necessarily an integer */
};

/* allocate a new stack structure with initial size max_items */
struct fish_stack *fish_alloc_stack(size_t max_items);

/* realloc stack data array to fit max_items; return true on success */
bool fish_resize_stack(struct fish_stack *stack, size_t max_items);

/* free a stack structure */
void fish_free_stack(struct fish_stack *stack);

#endif /* FISH_STACK_H */
