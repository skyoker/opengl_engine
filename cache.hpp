#pragma once

#include "utils.hpp"
#include "textures.hpp"

struct Cache {
    Chunks loaded_chunks;
    Textures loaded_textures;

    void add_chunk_to_cache(Chunk& chunk);
    bool is_chunk_loaded(const Chunk& chunk);
    void remove_chunk_from_cache(const Chunk& chunk);
    
    void add_tex_to_cache(Texture& tex);
    bool is_tex_loaded(const Texture& tex);
    void remove_tex_from_cache(const Texture& tex);
};