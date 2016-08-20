#ifndef CODEBOX_H
#define CODEBOX_H

#include <stddef.h>
#include <uthash.h>

/* the size of the codebox "core", stored in a 2-dimensional array */
#define CODEBOX_WIDTH 256
#define CODEBOX_HEIGHT 256

/* fish number datatype */
typedef int64_t fish_number;

/* instruction pointer direction; FINISHED denotes execution stopped */
enum fish_direction
{
    RIGHT, LEFT, DOWN, UP, FINISHED
};

/* execution state */
struct fish_state
{
    /* location of instruction pointer */
    size_t row;
    size_t column;

    /* direction of instruction pointer */
    enum fish_direction direction;

    /* uthash handle */
    UT_hash_handle hh;
};

/* hash states by their location and direction */
#define FISH_STATE_KEYLEN (offsetof(struct fish_state, direction) \
                           + sizeof(((struct fish_state *)0)->direction) \
                           - offsetof(struct fish_state, row))

/* codebox */
struct fish_codebox
{
    fish_number core[CODEBOX_HEIGHT][CODEBOX_WIDTH];
};

/* increment instruction pointer */
void fish_next(struct fish_state *state, struct fish_codebox const *codebox);

/* read a string starting at state, returning a malloced buffer and advancing
 * state to the ending quote */
fish_number *fish_read_string(struct fish_state *state,
                              struct fish_codebox const *codebox);

/* get a value from the codebox */
fish_number fish_get(struct fish_codebox const *codebox, long row, long column);

/* peek the next value that would be read by incrementing state */
fish_number fish_peek_next(struct fish_codebox const *codebox,
                           struct fish_state state);

/* set a value in the codebox */
void fish_set(struct fish_codebox *codebox, long row, long column,
              fish_number value);

#endif /* CODEBOX_H */
