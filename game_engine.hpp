#pragma once

#include "world.hpp"
#include "utils.hpp"
#include "engine.hpp"

struct GameEngine {
    int screen_width = 0;
    int screen_height = 0;
    int fps = 60;

    const int tiles_on_screenx = 15;
    const int tiles_on_screeny = 15;

    Vec2 tilesize_on_screen = {
        2.0f / static_cast<float>(tiles_on_screenx),
        2.0f / static_cast<float>(tiles_on_screeny)
    };

    Engine2D* engine = nullptr; // manually set this later
    World* world = nullptr;     // manually set this later

    // --- WINDOW STRUCT ---
    struct Window {
        World* world = nullptr;
        Vec2 window_pos;
        int window_sizex = 0;
        int window_sizey = 0;

        Chunks chunks_in_window(); // function
        Tiles tiles_in_window();

        Chunks chunks_in_win;
        Tiles tiles_in_win;
    };

    // --- PLAYER STRUCT ---
    struct Player {
        Engine2D* engine = nullptr;
        World* world = nullptr;

        Vec2 tilesize_on_screen;

        Tile isontile;
        Vec2 player_pos_on_screen;

        Vec2 chunkpos = isontile.chunk_pos;
        Vec2 tilepos = isontile.inside_chunk_pos;
        Vec2 worldpos = getworldcords(tilepos, chunkpos, world.tiles_per_chunk);

        void move_player(Vec2 amount);
        void DrawPlayer();
   
    };

    // --- ENGINE FUNCTIONS ---
    void StartEngine(); 
    void DrawTile(Vec2 pos, const Tile& tile, Engine2D& engine);
    void DrawWindow(const Window& window, Engine2D& engine, World& world);
};

