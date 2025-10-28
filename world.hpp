#pragma once
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;


namespace fs = std::filesystem;

struct World {
    fs::path path_to_world; 

    World(const fs::path& Worldpath) : path_to_world(Worldpath) {}

    std::map<std::string, std::map<std::string, std::string>> LoadChunk(int xpos, int ypos);
    std::map<std::string, std::string> GetTile(int xpos, int ypos, const std::map<std::string, std::map<std::string, std::string>>& chunk);
};
