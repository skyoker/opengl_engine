#pragma once

#include <iostream>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <filesystem>

namespace fs = std::filesystem;

struct Vec2 { float x, y; };
struct Vec3 { float r, g, b; };

enum class TileType {
    Null,
    Rock,
    Wall,
    RoomEdge,
    Room,
    Unknown,
};

enum class TexPath {
    Null,
    Rock,
    Wall,
    Room,
    Unknown,
};

inline TileType StringToTileType(const std::string& str) {
    if (str == "rock") return TileType::Rock;
    if (str == "wall") return TileType::Wall;
    if (str == "room") return TileType::Room;
    if (str == "roomedge") return TileType::RoomEdge;
    if (str == "NULL")  return TileType::Null;

    std::cerr << "no tiletype detected: " << '\n';
    return TileType::Unknown;
}

inline std::string TileTypeToString(TileType type) {
    switch (type) {
        case TileType::Rock: return "rock";
        case TileType::Wall: return "wall";
        case TileType::Room: return "room";
        case TileType::Null: return "NULL";
        case TileType::RoomEdge: return "roomedge";
        default: return "unknown";
    }
}

inline fs::path TexPathToPath(TexPath textype) {
    switch (textype) {
        case TexPath::Rock: return fs::path("../textures/tiles/rock.png");
        case TexPath::Wall: return fs::path("../textures/tiles/wall.png");
        case TexPath::Room: return fs::path("../textures/tiles/room.png");
        case TexPath::Null: return fs::path("../textures/tiles/null.png");
        default: return fs::path();
    }
}

inline TexPath TileTypeToTexPath(TileType type) {
    switch (type) {
        case TileType::Rock: return TexPath::Rock;
        case TileType::Wall: return TexPath::Wall;
        case TileType::Room: return TexPath::Room;
        case TileType::Null: return TexPath::Null;
        //case TileType::RoomEdge: return TexPath::Null;
        default: return TexPath::Unknown;
    }
}

inline int randomFromSeed(std::mt19937& gen, int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
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

struct Texture {
    fs::path path_to_image;              // Path to the PNG file
    TileType tt;                         // Tile type associated with this texture
    std::vector<float> vertices;         // Vertices stored as local UV + color: u, v, r, g, b ...
    int width = 0;                       // Image width in pixels
    int height = 0;                      // Image height in pixels

    Texture() : path_to_image(""), tt(TileType::Unknown), vertices(), width(0), height(0) {}
    Texture(fs::path path, TileType type, std::vector<float> verts, int w = 0, int h = 0)
        : path_to_image(path), tt(type), vertices(verts), width(w), height(h) {}
};

struct Textures {
    std::vector<Texture> textures;
    void add_tex(Texture tex);
    void clear_tex();
    bool contains(Texture tex);
    void remove(Texture tex);
};

struct Room {
    int width;
    int height;

    int world_posx;
    int world_posy;
};

struct Rooms {
    std::vector<Room> rooms;

    void add_room(Room& room_to_be_added);
    void add_rooms(Rooms& rooms_to_be_added);
    void clear_rooms();
    bool contains(const Room& room);
    void remove(const Room& room);
};

Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 subVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 multiVec2pos(const Vec2 pos1, const Vec2 pos2, float multi);

Vec2 addVec2posx(const Vec2 pos, float tobeadded);
Vec2 addVec2posy(const Vec2 pos, float tobeadded);


Vec2 getworldcords(Vec2 tilepos, Vec2 chunkpos, float tiles_per_chunk);
std::string get_chunk_string(int chunkx, int chunky);