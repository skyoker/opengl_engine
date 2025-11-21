#include "utils.hpp"

Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2) {
    float newx = pos1.x + pos2.x;
    float newy = pos1.y + pos2.y;

    Vec2 new_pos = { newx, newy};


    return new_pos;
}

Vec2 subVec2pos(const Vec2 pos1, const Vec2 pos2) {
    float newx = pos1.x - pos2.x;
    float newy = pos1.y - pos2.y;

    Vec2 new_pos = {newx, newy};


    return new_pos;
}

Vec2 addVec2posx(const Vec2 pos, float tobeadded) {
    Vec2 new_pos{pos.x + tobeadded, pos.y};

    return {pos.x + tobeadded, pos.y};
}

Vec2 addVec2posy(const Vec2 pos, float tobeadded) {
    Vec2 new_pos{pos.x, pos.y + tobeadded};

    return new_pos; 
}

Vec2 multiVec2pos(const Vec2 pos, float multiplier ) {
    Vec2 new_pos{pos.x * multiplier, pos.y * multiplier};

    return new_pos;
}

Vec2 getworldcords(Vec2 tilepos, Vec2 chunkpos, float tiles_per_chunk) {

    return addVec2pos(multiVec2pos(chunkpos, tiles_per_chunk), tilepos);

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

void Tiles::remove(const Tile& tile) {
    tiles.erase(
        std::remove_if(
            tiles.begin(),
            tiles.end(),
            [&](const Tile& t) {
                return t.inside_chunk_pos.x == tile.inside_chunk_pos.x &&
                       t.inside_chunk_pos.y == tile.inside_chunk_pos.y &&
                       t.chunk_pos.x == tile.chunk_pos.x &&
                       t.chunk_pos.y == tile.chunk_pos.y;
            }),
        tiles.end()
    );
}

Tile Tiles::get_tile(int tilex, int tiley) {
    for (const auto& t : tiles) {
        if (t.inside_chunk_pos.x == tilex && t.inside_chunk_pos.y == tiley) {
            return t;
        }
    }

    std::cerr << "Tile not found at: " << tilex << "x " << tiley << "y\n";

    // Return a default tile to avoid crash
    Tile defaultTile;
    defaultTile.type = TileType::Unknown;  // or whatever fits your system
    defaultTile.inside_chunk_pos = {static_cast<float>(tilex), static_cast<float>(tiley)};
    return defaultTile;
}

void Tiles::clear_tiles() {
    tiles.clear();

}

int randomFromSeed(int seed, int min, int max) {
    std::mt19937 generator(seed);            // Mersenne Twister seeded
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

std::string get_chunk_string(int chunkx, int chunky) {
    std::string out;
    out + "ch" + std::to_string(chunkx) + "x" + std::to_string(chunky) + "y.json"; 
    return out;
}