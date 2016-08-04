#ifndef FISH_RUNTIME_H
#define FISH_RUNTIME_H

struct fish_stack;

/* reverse stack ('r' instruction) */
void fish_reverse_stack(struct fish_stack *stack);

/* shift stack left ('{' instruction) */
void fish_shift_left(struct fish_stack *stack);

/* shift stack right ('}' instruction) */
void fish_shift_right(struct fish_stack *stack);

#endif /* FISH_RUNTIME_H */
