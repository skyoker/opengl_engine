#include "world.hpp"


std::map<std::string, std::map<std::string, std::string>> World::LoadChunk(int xpos, int ypos) {
    std::string chunkfilename = "ch" + std::to_string(xpos) + "x" + std::to_string(ypos) + "y.json";
    fs::path chunkpath = path_to_world / chunkfilename;

    std::ifstream file(chunkpath);
    if (!file.is_open()) {
        std::cerr << "Failed to open chunk: " << chunkpath << "\n";
        return {};
    }

    json j;
    file >> j;  // parse the JSON
    file.close();

    std::map<std::string, std::map<std::string, std::string>> chunkData;

    for (auto& [tileKey, tileValue] : j.items()) {
        std::map<std::string, std::string> tileMap;
        for (auto& [k, v] : tileValue.items()) {
            tileMap[k] = v.get<std::string>();
        }
        chunkData.at(tileKey) = tileMap;
    }

    return chunkData;
}

std::map<std::string, std::string> GetTile(int xpos, int ypos, std::map<std::string, std::map<std::string, std::string>>& chunk) {
    std::string tilekey = "t" + std::to_string(xpos) + "x" + std::to_string(ypos) + "y";
    auto tile = chunk.at(tilekey);
    return tile;

}
