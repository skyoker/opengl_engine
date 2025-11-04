#include "cache.hpp"


void Cache::add_chunk_to_cache(Chunk& chunk) {
    loaded_chunks.add_chunk(chunk);
}

bool Cache::is_chunk_loaded(const Chunk& chunk) {
    return loaded_chunks.contains(chunk);
}

void Cache::remove_chunk_from_cache(const Chunk& chunk) {
    loaded_chunks.remove_chunk(chunk);
}