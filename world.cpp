#include "world.hpp"
#include "utils.hpp"
#include "world.hpp"
#include "cache.hpp"
#include "utils.hpp"

Chunk World::LoadChunk(int xpos, int ypos) {
    // temp struct for cache check
    Chunk temp;
    temp.pos = {static_cast<float>(xpos), static_cast<float>(ypos)};

    // 1 check cache
    if (cache->is_chunk_loaded(temp)) {
        for (auto& cached_chunk : cache->loaded_chunks.chunks) {
            if (cached_chunk.pos.x == temp.pos.x && cached_chunk.pos.y == temp.pos.y) {
                return cached_chunk;
            }
        }
    }

    // 2 load from file
    std::string chunkfilename = get_chunk_string(xpos, ypos);

    fs::path chunkpath = *path_to_world / chunkfilename;

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

    // 3 parse tiles
    for (auto& [tileKey, tileValue] : j.items()) {
        Tile tile;

        // Parse key: "t0x6y"
        int tx = 0, ty = 0;
        sscanf(tileKey.c_str(), "t%dx%dy", &tx, &ty);

        tile.inside_chunk_pos.x = tx;
        tile.inside_chunk_pos.y = ty;
        tile.chunk_pos = chunk.pos;

        // Extract tile type string
        std::string type_str = tileValue["type"];

        // Convert to enum
        tile.type = StringToTileType(type_str);

        chunk.tiles.add_tile(tile);
    }

    // 4 add to cache
    cache->add_chunk_to_cache(chunk);

    // 5 return loaded chunk
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

Tile World::GetTileGlobal(int world_x, int world_y) {
    int chunk_x = world_x / tiles_per_chunk;
    int chunk_y = world_y / tiles_per_chunk;
    int tile_x = world_x % tiles_per_chunk;
    int tile_y = world_y % tiles_per_chunk;

    // Make sure we have this chunk loaded (from cache or from file)
    Chunk chunk = LoadChunk(chunk_x, chunk_y);

    // Assuming your Tiles struct has a method to fetch a specific tile
    return chunk.tiles.get_tile(tile_x, tile_y);
}

void World::init() {
    // Validate all required pointers are set
    if (!cache) {
        std::cerr << "ERROR World::init(): cache pointer not set!\n";
        std::exit(1);
    }
    if (!path_to_world) {
        std::cerr << "ERROR World::init(): path_to_world pointer not set!\n";
        std::exit(1);
    }

    // Load world info from disk from JSON files 
    std::string filename = "world_info.json";
    fs::path filepath = *path_to_world / filename;
    
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
    

    // Set spawntile (where the player starts)
    // spawnpoint_x and spawnpoint_y are world coordinates (0 to tiles_per_chunk*chunks_per_worldx).
    // Use GetTileGlobal which automatically converts world coords to chunk and tile-within-chunk.
    int spawn_world_x = j.value("spawnpoint_x", 0);
    int spawn_world_y = j.value("spawnpoint_y", 0);
    spawntile = GetTileGlobal(spawn_world_x, spawn_world_y);

}