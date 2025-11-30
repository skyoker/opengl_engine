#include "cache.hpp"


void Cache::add_chunk_to_cache(Chunk& chunk) {
    loaded_chunks.add_chunk(chunk);
}

bool Cache::is_chunk_loaded(const Chunk& chunk) {
    return loaded_chunks.contains(chunk);
}

void Cache::remove_chunk_from_cache(const Chunk& chunk) {
    loaded_chunks.remove(chunk);
    
}

void Cache::add_tex_to_cache(Texture& tex) {
    loaded_textures.add_tex(tex);
}

bool Cache::is_tex_loaded(const Texture& tex) {
    return loaded_textures.contains(tex);
}

void Cache::remove_tex_from_cache(const Texture& tex) {
    loaded_textures.remove(tex);
    
}