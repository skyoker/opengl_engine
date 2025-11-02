#pragma once

#include <map>
#include <iostream>

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


Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 addVec2posx(const Vec2 pos, float tobeadded);
Vec2 addVec2posy(const Vec2 pos, float tobeadded);
