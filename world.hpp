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

struct Tile {
    Vec2 inside_chunk_pos;
    Vec2 chunk_pos;
    TileType type;

};

struct Tiles {
    std::vector<Tile> tiles;

    void add_tile(const Tile& tile_to_be_added);
    void clear_tiles();
    void remove(const Tile& tile);
    Tile get_tile(int tilex, int tiley);
};

struct Chunk {
    std::string name;
    Vec2 pos;
    Tiles tiles;
};

struct Chunks {
    std::vector<Chunk> chunks;

    void add_chunk(Chunk& chunk_to_be_added);
    void clear_chunks();
    bool contains(const Chunk& chunk);
    void remove(const Chunk& chunk);

};

class Cache; // forward declaration becouse cache.hpp includes world.hpp which would cause a cyclic dependency

struct World {

    fs::path* path_to_world; // define this at first instance
    Cache* cache; // this will be set in init()


    Chunk LoadChunk(int xpos, int ypos);
    Tile GetTile(int xpos, int ypos, const Chunk& chunk);
    Tile GetTileGlobal(int worldx, int worldy);

    int tiles_per_chunk; // this will be set by init()
    int chunks_per_worldx; // this will be set by init()
    int chunks_per_worldy; // this will be set by init()

    Tile spawntile = GetTile(
        tiles_per_chunk / 2,
        tiles_per_chunk / 2,
        LoadChunk(chunks_per_worldx / 2, chunks_per_worldy / 2)
    );

    void get_info();

    void init();

};

