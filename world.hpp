#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>

#include "utils.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;


struct Chunk {
    std::string name;
    int x;
    int y;
    Tiles tiles;
};

struct Tile {
    int x;
    int y;
    std::string type;

};

struct Chunks {
    std::vector<Chunk> chunks;

    void add_chunk(Chunk& chunk_to_be_added);
    void clear_chunks();
};

struct Tiles {
    std::vector<Tile> tiles;

    void add_tile(const Tile& tile_to_be_added);
    void clear_tiles();
};


struct World {
    const Vec2 spawnpoint = {0, 0};

    fs::path path_to_world; 

    World(const fs::path& Worldpath) : path_to_world(Worldpath) {}

    Chunk LoadChunk(int xpos, int ypos);
    Tile GetTile(int xpos, int ypos, const Chunk& chunk);

    int tiles_per_chunk;
    int chunks_per_worldx;
    int chunks_per_worldy;

    private:
    void get_info();

};



