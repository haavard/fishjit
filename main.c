#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <termios.h>

#include "fish-cache.h"
#include "fish-codebox.h"
#include "fish-stack.h"

int main(int argc, char *argv[])
{
    /* seed PRNG */
    srand(time(NULL));

    /* disable input echo (for 'i' instruction) */
    struct termios t;
    tcgetattr(fileno(stdout), &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(fileno(stdout), TCSANOW, &t);

    /* validate command-line arguments */
    if (argc < 2)
    {
        fputs("No input file provided", stderr);
        exit(EXIT_FAILURE);
    }

    /* open script file */
    FILE *script_file = fopen(argv[1], "r");
    if (script_file == NULL)
    {
        perror("Could not open input file");
        exit(EXIT_FAILURE);
    }

    /* read script file */
    struct fish_codebox codebox = {0};
    char line[CODEBOX_WIDTH];

    size_t row = 0;
    while (fgets(line, sizeof line, script_file))
    {
        /* read and discard until end of line, if necessary */
        /* TODO: don't do this */
        while (!strchr(line, '\n'))
            fgets(line, sizeof line, script_file);

        /* store line into codebox, excluding the newline character */
        size_t len = strlen(line) - 1;
        for (size_t column = 0; column < len; ++column)
        {
            fish_set(&codebox, row, column, line[column]);
        }

        ++row;
    }

    if (ferror(script_file))
    {
        perror("Could not read from input file");
        exit(EXIT_FAILURE);
    }

    if (fclose(script_file) == EOF)
    {
        perror("Could not close input file");
        exit(EXIT_FAILURE);
    }

    /* initialize instruction cache and VM state */
    struct fish_cache *cache = NULL;
    struct fish_state state = { .row = 0, .column = 0, .direction = RIGHT };
    size_t const initial_stack_size = 128;
    struct fish_stack *stack = fish_alloc_stack(initial_stack_size);

    /* main loop */
    do
    {
        /* get cached code for current state or (re)compile it */
        struct fish_code *code = fish_get_or_compile(&cache, &state, &codebox);

        if (code == NULL)
        {
            /* no code means it could not be assembled */
            fputs("Syntax error\n", stderr);
            exit(EXIT_FAILURE);
        }
        else
        {
            /* grow stack to twice the size if an overflow is possible */
            size_t potential_items = stack->num_items + code->max_stack_change;
            size_t new_size = 0;
            if (potential_items > stack->max_items)
            {
                new_size = stack->max_items * 2;
            }
            /* shrink stack to half the size if less than a third is used */
            else if (stack->max_items > initial_stack_size
                     && potential_items < stack->max_items / 3)
            {
                new_size = stack->max_items / 2;
            }

            if (new_size && !fish_resize_stack(stack, new_size))
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }

            /* execute code, updating state */
            int ret = code->entry(&state, stack);
            fflush(NULL);
            if (ret != 0)
            {
                fputs("something smells fishy...\n", stderr);
                break;
            }
        }
    }
    while (state.direction != FINISHED);

    /* clean up */
    fish_free_cache(cache);
    fish_free_stack(stack);

    return 0;
}
