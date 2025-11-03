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