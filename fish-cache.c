#include <stdio.h>

#include "fish-codebox.h"

#include "fish-cache.h"

struct fish_code *fish_get_or_compile(struct fish_cache **cache,
                                      struct fish_state *state,
                                      struct fish_codebox *codebox)
{
    /* check cache */
    struct fish_cache *cache_item = NULL;
    HASH_FIND(hh, *cache, &(state->row), FISH_STATE_KEYLEN, cache_item);
    if (cache_item)
        return cache_item->code;

    /* compile code */
    struct fish_code *code = fish_compile(codebox, *state);
    if (code == NULL)
        return NULL;

    /* store compiled code in cache */
    cache_item = malloc(sizeof(struct fish_cache));
    if (!cache_item)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    cache_item->state = *state;
    cache_item->code = code;
    HASH_ADD(hh, *cache, state, FISH_STATE_KEYLEN, cache_item);

    return code;
}

void fish_free_cache(struct fish_cache *cache)
{
    struct fish_cache *item, *tmp;
    HASH_ITER(hh, cache, item, tmp)
    {
        HASH_DEL(cache, item);
        fish_free_code(item->code);
        free(item);
    }
}
