#ifndef FISH_STACK_H
#define FISH_STACK_H

#include <stdlib.h>
#include <stdbool.h>

#include "fish-codebox.h"

/* size of one item on the stack */
#define FISH_STACK_ITEM_SIZE (9)

/*
 * The fish stack data consists of alternating numbers and type identifiers.
 * The numbers are 64-bit integers or doubles. The type identifier is a byte
 * located after each number, making each item a total of 9 bytes large.
 */

/* constants identifying the type of stack items */
enum fish_type {
    INTEGER = 0,
    FLOAT = 1
};

/* fish interpreter stack (and register) */
struct fish_stack
{
    uint64_t num_items;
    uint64_t max_items;
    void *data;

    /* register holds a single item */
    uint8_t register_set;
    uint8_t register_type;
    int64_t register_value; /* not necessarily an integer */
};

/* allocate a new stack structure */
struct fish_stack *fish_alloc_stack();

/* realloc stack data array to fit max_items; return true on success */
bool fish_resize_stack(struct fish_stack *stack, size_t max_items);

/* free a stack structure */
void fish_free_stack(struct fish_stack *stack);

#endif /* FISH_STACK_H */
