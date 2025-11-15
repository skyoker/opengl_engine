#pragma once

#include <iostream>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>





struct Vec2 { float x, y; };
struct Vec3 { float r, g, b; };

enum class TileType {
    Null,
    Rock,
    Wall,
    Unknown
};

inline TileType StringToTileType(const std::string& str) {
    if (str == "rock") return TileType::Rock;
    if (str == "water") return TileType::Wall;
    if (str == "NULL")  return TileType::Null;

    std::cerr << "no tiletype detected: " << '\n';
    return TileType::Unknown;
}

inline std::string TileTypeToString(TileType type) {
    switch (type) {
        case TileType::Rock: return "rock";
        case TileType::Wall: return "wall";
        case TileType::Null: return "null";
        default: return "unknown";
    }
}

template<typename K, typename V>
const V* safeloc(const std::map<K, V>& map, const K& key) {
    auto it = map.find(key);
    if (it != map.end())
        return &it->second;

    std::cerr << "Missing key: " << key << '\n';
    return nullptr;
}

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

Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 subVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 multiVec2pos(const Vec2 pos1, const Vec2 pos2, float multi);

Vec2 addVec2posx(const Vec2 pos, float tobeadded);
Vec2 addVec2posy(const Vec2 pos, float tobeadded);


Vec2 getworldcords(Vec2 tilepos, Vec2 chunkpos, float tiles_per_chunk);
