#pragma once
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

#include "utils.hpp"

using json = nlohmann::json;

namespace fs = std::filesystem;

struct Chunk {
    std::string name;
    int x;
    int y;
    std::map<std::string, std::map<std::string, std::string>> tiles;
};

struct Tile {
    int x;
    int y;
    std::string type;

};

struct World {
    int player_init_posx;
    int player_init_posy;

    fs::path path_to_world; 

    World(const fs::path& Worldpath) : path_to_world(Worldpath) {}

    Chunk LoadChunk(int xpos, int ypos);
    Tile GetTile(int xpos, int ypos, const Chunk& chunk);
};


