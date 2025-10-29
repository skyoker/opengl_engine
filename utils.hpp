#include <map>
#include <iostream>

struct Vec2 { float x, y; };
struct Vec3 { float r, g, b; };

template<typename K, typename V>
V* safeloc(std::map<K, V>& map, const K& key) {
    auto it = map.find(key);
    if (it != map.end())
        return &it->second;

    std::cerr << "Missing key: " << key << '\n';
    return nullptr;
}

Vec2 addVec2pos(const Vec2 pos1, const Vec2 pos2);
Vec2 addVec2posx(const Vec2 pos, float tobeadded);
Vec2 addVec2posy(const Vec2 pos, float tobeadded);
