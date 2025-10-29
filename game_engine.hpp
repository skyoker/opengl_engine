#pragma once

#include "world.hpp"
#include "utils.hpp"


/*

the game engine handels:
1. the drawing of the current screen 
2. game loop
3. player pos and drawing


*/
struct Player {
    int chunkx;
    int chunky;
    int tilex;
    int tiley;

    Vec2 player_pos;

    void move_player(Vec2 amount);

    // void Drawplayer()
};

struct GameEngine {
    int screen_width;
    int screen_height;
    World world;

    GameEngine(const int screenwidth, const int screenheight, World world_i) : screen_height(screenheight), screen_width(screen_width), world(world_i) {}

    void StartEngine();


};