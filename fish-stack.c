#include <stdio.h>

#include "fish-stack.h"

/* bytes needed to store items: 1 (identifier) + 8 (value) */
#define STACK_SIZE(items) (9 * (items))

/* allocate a new stack structure with initial size max_items */
struct fish_stack *fish_alloc_stack(size_t max_items)
{
    struct fish_stack *stack = malloc(sizeof(struct fish_stack));
    fish_number *data = malloc(STACK_SIZE(max_items));
    if (!stack || !data)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    stack->data = data;
    stack->max_items = max_items;
    stack->num_items = 0;
    stack->register_set = false;

    return stack;
}

/* realloc stack data array to fit max_items; return true on success */
bool fish_resize_stack(struct fish_stack *stack, size_t max_items)
{
    fish_number *data = realloc(stack->data, STACK_SIZE(max_items));

    if (data)
    {
        stack->data = data;
        stack->max_items = max_items;

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
void fish_free_stack(struct fish_stack *stack)
{
    free(stack->data);
    free(stack);
}
