#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>

#include "utils.hpp"

namespace fs = std::filesystem;

struct GenWorld {
    const int TILES_PER_CHUNK = 8;
    const int CHUNKS_PER_WORLDX = 10;
    const int CHUNKS_PER_WORLDY = 10;

    const fs::path FOLDER_PATH = "../world";

    void make_world_folder();
    void create_chunk_file(int chunk_x, int chunk_y);
    void create_meta_file(int tiles_per_chunk, int chunks_per_worldx, int chunks_per_worldy);
    void insert_tile_on_chunk(fs::path chunk_path, Vec2 tp, TileType tt);


    void random_wall_placement(float randomness);


    void generate_world();
};

