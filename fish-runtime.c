#include "fish-stack.h"

#include "fish-runtime.h"

/* reverse stack ('r' instruction) */
void fish_reverse_stack(struct fish_stack *stack)
{
    void *front = stack->data;
    void *back = stack->data + FISH_STACK_ITEM_SIZE * (stack->num_items - 1);

    while (back - front >= FISH_STACK_ITEM_SIZE)
    {
        /* swap front and back and move inwards */
        char buf[FISH_STACK_ITEM_SIZE];
        memcpy(buf, front, FISH_STACK_ITEM_SIZE);
        memcpy(front, back, FISH_STACK_ITEM_SIZE);
        memcpy(back, buf, FISH_STACK_ITEM_SIZE);

        front += FISH_STACK_ITEM_SIZE;
        back -= FISH_STACK_ITEM_SIZE;
    }
}

/* shift stack left ('{' instruction) */
void fish_shift_left(struct fish_stack *stack)
{
    void *front = stack->data;
    void *back = stack->data + FISH_STACK_ITEM_SIZE * (stack->num_items - 1);

    /* copy front, shift everything else to front and put front in back */
    char buf[FISH_STACK_ITEM_SIZE];
    memcpy(buf, front, FISH_STACK_ITEM_SIZE);
    memmove(front, front + FISH_STACK_ITEM_SIZE,
            (stack->num_items - 1) * FISH_STACK_ITEM_SIZE);
    memcpy(back, buf, FISH_STACK_ITEM_SIZE);
}

/* shift stack right ('}' instruction) */
void fish_shift_right(struct fish_stack *stack)
{
    void *front = stack->data;
    void *back = stack->data + FISH_STACK_ITEM_SIZE * (stack->num_items - 1);

    /* copy back, shift everything else to front and put front in back */
    char buf[FISH_STACK_ITEM_SIZE];
    memcpy(buf, back, FISH_STACK_ITEM_SIZE);
    memmove(front + FISH_STACK_ITEM_SIZE, front,
            (stack->num_items - 1) * FISH_STACK_ITEM_SIZE);
    memcpy(front, buf, FISH_STACK_ITEM_SIZE);
}
