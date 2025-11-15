#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>

#include "utils.hpp"
#include "cache.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;


struct World {
    Cache* cache; // define this at first instance

    fs::path* path_to_world; // define this at first instance
    int tiles_per_chunk; // this will be set by init()
    int chunks_per_worldx; // this will be set by init()
    int chunks_per_worldy; // this will be set by init()

    Tile spawntile; // this will be set by init()


    Chunk LoadChunk(int xpos, int ypos);
    Tile GetTile(int xpos, int ypos, const Chunk& chunk);
    Tile GetTileGlobal(int worldx, int worldy);

    void init();

};

