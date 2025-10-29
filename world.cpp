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
            tileMap[k] = v.get<std::string>();
        }
        chunkData[tileKey] = tileMap;
    }

    Chunk chunk;
    chunk.x = xpos;
    chunk.y = ypos;
    chunk.name = chunkfilename;
    chunk.tiles = chunkData;

    return chunk;
}

Tile World::GetTile(int xpos, int ypos, const Chunk& chunk) {
    std::string tilekey = "t" + std::to_string(xpos) + "x" + std::to_string(ypos) + "y";
    Tile tile;
    tile.x = xpos;
    tile.y = ypos;

    // safely find the tile
    auto tilePtr = safeloc(chunk.tiles, tilekey);
    if (!tilePtr) return tile; // missing tile, return empty tile

    // safely find "type" key inside tile
    std::string type_str = "type";
    auto typePtr = safeloc(*tilePtr, type_str);
    if (typePtr)
        tile.type = *typePtr;

    return tile;
}

void Chunks::add_chunk(Chunk& chunk_to_be_added) {

    chunks.push_back(chunk_to_be_added);
}

void Chunks::clear_chunks() {
    chunks.clear();
}