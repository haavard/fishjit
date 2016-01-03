#ifndef FISH_CACHE_H
#define FISH_CACHE_H

#include <uthash.h>

#include "fish-compiler.h"

struct fish_cache
{
    struct fish_state state;

    struct fish_code *code;

    UT_hash_handle hh;
};

/* fetch compiled code from a cache, or (re)compile it if needed */
struct fish_code *fish_get_or_compile(struct fish_cache **cache,
                                      struct fish_state *state,
                                      struct fish_codebox *codebox);

/* free an entire cache structure */
void fish_free_cache(struct fish_cache *cache);

#endif /* FISH_CACHE_H */
