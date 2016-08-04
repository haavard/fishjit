#include "fish-stack.h"
#include "fish-codebox.h"
#include <stdio.h>

/* initial amount of space on the stack, in items */
#define FISH_INITIAL_STACK_SIZE (128)

/* bytes needed to store items: 1 (identifier) + 8 (value) */
#define STACK_SIZE(items) (FISH_STACK_ITEM_SIZE * (items))

/* allocate a new stack structure with initial size max_items */
struct fish_stack *
fish_alloc_stack()
{
    struct fish_stack *stack = malloc(sizeof(struct fish_stack));
    fish_number *data = malloc(STACK_SIZE(FISH_INITIAL_STACK_SIZE));
    if (!stack || !data)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    stack->data = data;
    stack->max_items = FISH_INITIAL_STACK_SIZE;
    stack->num_items = 0;
    stack->register_set = false;

    return stack;
}

/* realloc stack data array to fit items; return true on success */
bool
fish_resize_stack(struct fish_stack *stack, size_t items)
{
    size_t new_size = stack->max_items;

    /* grow stack to be large enough by doubling its size */
    while (items > new_size)
        new_size *= 2;

    /* if less than a third of the stack is used, halve its size */
    while (new_size > FISH_INITIAL_STACK_SIZE && items < new_size / 3)
        new_size /= 2;

    /* nothing to be done */
    if (new_size == stack->max_items)
        return true;

    /* resize data buffer */
    fish_number *data = realloc(stack->data, STACK_SIZE(new_size));

    /* if resize was successful */
    if (data)
    {
        stack->data = data;
        stack->max_items = new_size;

        /* consistency check */
        if (stack->num_items > stack->max_items)
        {
            fputs("warning: stack truncated\n", stderr);
            stack->num_items = stack->max_items;
        }

        return true;
    }

    return false;
}

/* free a stack structure */
void
fish_free_stack(struct fish_stack *stack)
{
    free(stack->data);
    free(stack);
}
