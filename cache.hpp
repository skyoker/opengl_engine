#pragma once

#include "world.hpp"

struct Cache {
    Chunks loaded_chunks;

    void add_chunk_to_cache(Chunk& chunk);
    bool is_chunk_loaded(const Chunk& chunk);
    void remove_chunk_from_cache(const Chunk& chunk);
};