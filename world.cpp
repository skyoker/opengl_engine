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
    chunk.pos = {static_cast<float>(xpos), static_cast<float>(ypos)};
    chunk.name = chunkfilename;

    // convert JSON tiles into Tile structs
    for (auto& [tileKey, tileValue] : j.items()) {
        Tile tile;
        tile.inside_chunk_pos.x = tileValue.value("x", 0);
        tile.inside_chunk_pos.y = tileValue.value("y", 0);
        tile.chunk_pos = chunk.pos;
        tile.type = StringToTileType(tileValue.value("type", "null"));

        chunk.tiles.add_tile(tile);
    }

    return chunk;
}

Tile World::GetTile(int xpos, int ypos, const Chunk& chunk) {
    Tile tile;
    tile.inside_chunk_pos.x = xpos;
    tile.inside_chunk_pos.y = ypos;
    tile.chunk_pos = chunk.pos;
    
    // Find the matching tile in the chunk
    for (const auto& t : chunk.tiles.tiles) {
        if (t.inside_chunk_pos.x == xpos && t.inside_chunk_pos.y == ypos) {
            return t;
        }
    }
    
    // If no tile found, return unknown type
    tile.type = TileType::Unknown;
    return tile;
}

void Chunks::add_chunk(Chunk& chunk_to_be_added) {

    chunks.push_back(chunk_to_be_added);
}

void Chunks::clear_chunks() {


    chunks.clear();
}

bool Chunks::contains(const Chunk& chunk) {
    for (const auto& c : chunks) {
        if (c.pos.x == chunk.pos.x && c.pos.y == chunk.pos.y)
            return true;
    }
    return false;
}

void Chunks::remove(const Chunk& chunk) {
    chunks.erase(
        std::remove_if(
            chunks.begin(),
            chunks.end(),
            [&](const Chunk& c) {
                return c.pos.x == chunk.pos.x && c.pos.y == chunk.pos.y;
            }),
        chunks.end()
    );
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

Tile World::GetTileGlobal(int world_x, int world_y) {
    int chunk_x = world_x / tiles_per_chunk;
    int chunk_y = world_y / tiles_per_chunk;
    int tile_x = world_x % tiles_per_chunk;
    int tile_y = world_y % tiles_per_chunk;

    return chunks[{chunk_x, chunk_y}].tiles.get_tile(tile_x, tile_y);
}

