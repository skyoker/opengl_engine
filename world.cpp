#include "world.hpp"
#include "utils.hpp"

Chunk World::LoadChunk(int xpos, int ypos) {
    std::string chunkfilename = "ch" + std::to_string(xpos) + "x" + std::to_string(ypos) + "y.json";
    fs::path chunkpath = path_to_world / chunkfilename;

    std::ifstream file(chunkpath);
    if (!file.is_open()) {
        std::cerr << "Failed to open chunk: " << chunkpath << "\n";
        return {};
    }

    json j;
    file >> j;
    file.close();

    std::map<std::string, std::map<std::string, std::string>> chunkData;

    for (auto& [tileKey, tileValue] : j.items()) {
        std::map<std::string, std::string> tileMap;
        for (auto& [k, v] : tileValue.items()) {
            tileMap[k] = v.get<std::string>();  // safe insert
        }
        chunkData[tileKey] = tileMap;  // safe insert
    }

    Chunk chunk;
    chunk.x = xpos;
    chunk.y = ypos;
    chunk.name = chunkfilename;
    chunk.tiles = chunkData;

    return chunk;
}

Tile GetTile(int xpos, int ypos, Chunk chunk) {

    std::string tilekey = "t" + std::to_string(xpos) + "x" + std::to_string(ypos) + "y";
    auto tilePtr = safeloc(chunk.tiles, tilekey);
    Tile tile;
    tile.type = tilePtr.
    
    if (tilePtr)
        return *tilePtr;
    else
        std::cerr << "Could not find tile " << tilekey << " in chunk " << chunk.name << "\n";
}


