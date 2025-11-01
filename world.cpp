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

    Chunk chunk;
    chunk.x = xpos;
    chunk.y = ypos;
    chunk.name = chunkfilename;

    // convert JSON tiles into Tile structs
    for (auto& [tileKey, tileValue] : j.items()) {
        Tile tile;
        tile.x = tileValue.value("x", 0);
        tile.y = tileValue.value("y", 0);
        tile.type = tileValue.value("type", "NULL");

        chunk.tiles.add_tile(tile);  // use your Tiles::add_tile method
    }

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

void Tiles::add_tile(const Tile& tile_to_be_added) {

    tiles.push_back(tile_to_be_added);
}

void Tiles::clear_tiles() {
    tiles.clear();

}

void World::get_info() {
    std::string filename = "world_info.json";
    fs::path filepath = path_to_world / filename;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open world info: " << filepath << "\n";
        return; // return empty/default
    }

    json j;
    file >> j;
    file.close();

    tiles_per_chunk   = j.value("tiles_per_chunk", 0);
    chunks_per_worldx = j.value("chunks_per_worldx", 0);
    chunks_per_worldy = j.value("chunks_per_worldy", 0);

}