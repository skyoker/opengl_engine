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