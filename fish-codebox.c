#include <stdlib.h>
#include <stdio.h>
#include "fish-codebox.h"

void fish_next(struct fish_state *state, struct fish_codebox const *codebox)
{
    do
    {
        /* wrap around if appropriate */
        if (state->row == 0 && state->direction == UP)
            state->row = CODEBOX_HEIGHT - 1;
        else if (state->row >= CODEBOX_HEIGHT && state->direction == DOWN)
            state->row = 0;
        else if (state->column == 0 && state->direction == LEFT)
            state->column = CODEBOX_WIDTH - 1;
        else if (state->column >= CODEBOX_WIDTH && state->direction == RIGHT)
            state->column = 0;
        else
        {
            /* increment in correct direction */
            switch (state->direction)
            {
                case RIGHT:
                    ++(state->column);
                    break;
                case LEFT:
                    --(state->column);
                    break;
                case DOWN:
                    ++(state->row);
                    break;
                case UP:
                    --(state->row);
                    break;
                case FINISHED:
                    /* nothing */
                    break;
            }
        }
    }
    /* skip zeroes */
    while (fish_get(codebox, state->row, state->column) == '\0');
}

fish_number *fish_read_string(struct fish_state *state,
                              struct fish_codebox const *codebox)
{
    char string_mode = fish_get(codebox, state->row, state->column);

    /* characters of the string read */
    size_t i = 0;

    /* allocate an initial buffer to hold the string */
    size_t buffer_size = 64;
    fish_number *string = malloc(buffer_size * sizeof(fish_number));
    if (!string)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* move to first character in string */
    fish_next(state, codebox);

    for (;;)
    {
        /* read and store character; break if it's end of string */
        fish_number c = fish_get(codebox, state->row, state->column);

        if (c == 0)
            continue;
        if (c == string_mode)
            break;

        string[i++] = c;

        /* realloc buffer if needed */
        if (i == buffer_size)
        {
            buffer_size *= 2;
            fish_number *new_string =
                realloc(string, buffer_size * sizeof(fish_number));
            if (!new_string)
            {
                free(string);
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            string = new_string;
        }

        fish_next(state, codebox);
    }

    /* null-terminate */
    string[i] = '\0';

    return string;
}

fish_number fish_get(struct fish_codebox const *codebox, long row, long column)
{
    if (row >= 0 && row < CODEBOX_HEIGHT
        && column >= 0 && column < CODEBOX_WIDTH)
    {
        return codebox->core[row][column];
    }
    else
    {
        /* TODO */
        return 0;
    }
}

fish_number fish_peek_next(struct fish_codebox const *codebox,
                           struct fish_state state)
{
    fish_next(&state, codebox);
    return fish_get(codebox, state.row, state.column);
}

void fish_set(struct fish_codebox *codebox, long row, long column,
              fish_number value)
{
    if (row >= 0 && row < CODEBOX_HEIGHT
        && column >= 0 && column < CODEBOX_WIDTH)
    {
        codebox->core[row][column] = value;
    }
    else
    {
        /* TODO */
    }
}
