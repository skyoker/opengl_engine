#pragma once

#include "world.hpp"
#include "utils.hpp"

/*
The game engine handles:
1. Drawing of the current screen
2. Game loop
3. Player pos and drawing
4. Animations
5. Chunk loading
6. Interactions
7. Inputs
*/

struct Player {
    int chunkx;
    int chunky;
    int tilex;
    int tiley;

    Vec2 player_pos;

    void move_player(Vec2 amount);
    // void DrawPlayer();
};

struct GameEngine {
    int screen_width;
    int screen_height;
    int fps;

    World world;

    GameEngine(int screenwidth, int screenheight, const World& world_i)
        : screen_width(screenwidth), screen_height(screenheight), world(world_i) {}

    void StartEngine(); // main loop

    struct Window {
        Window(Vec2 pos, int size)
        : window_pos(pos), window_size(size) {}

        Vec2 window_pos;   // top-left corner
        int window_size;
        World world;

        Chunks chunks_in_window();
        int tiles_across;
    };

    void DrawWindow(const Window& window);
};
