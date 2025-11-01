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

    const int tiles_on_screenx = 15;
    const int tiles_on_screeny = 15;

    World world;

    GameEngine(int screenwidth, int screenheight, const World& world_i)
        : screen_width(screenwidth), screen_height(screenheight), world(world_i) {}

    void StartEngine(); // main loop

    struct Window {
        
        Window(Vec2 pos, int size)
        : window_pos(pos), window_size(size) {}

        Tiles tiles_in_win =  tiles_in_window();



        private:
        Vec2 window_pos;   // top-left corner
        int window_size; // in unit Tiles

        World world;
        Chunks chunks_in_window(); // function
        Tiles tiles_in_window();

        Chunks chunks_in_win = chunks_in_window();


    };


    void DrawTile(Vec2 pos, );
    void DrawChunk()
    void DrawWindow(const Window& window);

};
