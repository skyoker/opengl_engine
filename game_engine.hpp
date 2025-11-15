#pragma once

#include "world.hpp"
#include "utils.hpp"
#include "engine.hpp"

struct GameEngine {
    int screen_width; // define this at start of game engine instance
    int screen_height; // define this at start of game engine instance
    int* fps = nullptr; // define this at start of game engine instance

    int* tiles_on_screenx = nullptr; // define this at start of game engine instance    
    int* tiles_on_screeny = nullptr; // define this at start of game engine instance  

    Vec2 tilesize_on_screen; // this will be calculated in init()

    Engine2D* engine = nullptr; // define this at start of game engine instance
    World* world = nullptr; // define this at start of game engine instance

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
        Engine2D* engine = nullptr; // define this at first instance
        World* world = nullptr; // define this at first instance


        Vec2 tilesize_on_screen; // defined in init() --usage for drawing player
        Tile isontile; // defined in init() --usage for drawing player and for movement
        Vec2 player_pos_on_screen; // defined in init() --usage for drawing player

        void move_player(Vec2 amount, World& world);
        void DrawPlayer();

        void init();
    };

    // --- ENGINE FUNCTIONS ---
    void StartEngine(); 
    void DrawTile(Vec2 pos, const Tile& tile, Engine2D& engine);
    void DrawWindow(const Window& window, Engine2D& engine, World& world);
    
    // -- INIT -- 
    // to set calculated consts
    void init();

};

