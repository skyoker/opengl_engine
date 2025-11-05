
#include "game_engine.hpp"
#include <iostream>

// --- PLAYER ---

void GameEngine::Player::move_player(Vec2 amount, World& world) {
    // update world position
    worldpos = addVec2pos(worldpos, amount);

    // recalc chunk + tile positions
    chunkpos.x = static_cast<int>(worldpos.x) / world.tiles_per_chunk;
    chunkpos.y = static_cast<int>(worldpos.y) / world.tiles_per_chunk;

    tilepos.x = static_cast<int>(worldpos.x) % world.tiles_per_chunk;
    tilepos.y = static_cast<int>(worldpos.y) % world.tiles_per_chunk;

    // grab the tile directly (assuming you’ve got something like GetTileGlobal or GetTileAt)
    isontile = world.GetTileGlobal(
        static_cast<int>(worldpos.x),
        static_cast<int>(worldpos.y)
    );

    
}


void GameEngine::Player::DrawPlayer() {
    Vec3 player_color = {1.0f, 0.0f, 0.0f}; // red

    if (!engine || !engine->isRunning()) {
        std::cerr << "Engine not running, cannot draw player\n";
        return;
    }

    engine->drawRect(
        tilesize_on_screen.x,
        tilesize_on_screen.y,
        player_color,
        player_pos_on_screen
    );
}

void GameEngine::Player::init() {
    chunkpos = isontile.chunk_pos;
        tilepos = isontile.inside_chunk_pos;
        if (world) {
            worldpos = getworldcords(tilepos, chunkpos, world->tiles_per_chunk);
        }
    }

// --- WINDOW ---

Chunks GameEngine::Window::chunks_in_window() {
    int win_startx = static_cast<int>(window_pos.x);
    int win_starty = static_cast<int>(window_pos.y);
    int win_endx = win_startx + window_sizex;
    int win_endy = win_starty + window_sizey;

    Chunks chunks_in_win;

    if (!world) return chunks_in_win;

    for (int cx = win_startx / (world->tiles_per_chunk * window_sizex);
         cx <= win_endx / (world->tiles_per_chunk * window_sizex); cx++) {
        for (int cy = win_starty / (world->tiles_per_chunk * window_sizey);
             cy <= win_endy / (world->tiles_per_chunk * window_sizey); cy++) {

            Chunk chunk = world->LoadChunk(cx, cy);
            chunks_in_win.add_chunk(chunk);
        }
    }

    return chunks_in_win;
}

Tiles GameEngine::Window::tiles_in_window() {
    Tiles tiles_in_win;

    for (const auto& chunk : chunks_in_win.chunks) {
        for (const auto& tile : chunk.tiles.tiles) {
            tiles_in_win.add_tile(tile);
        }
    }

    return tiles_in_win;
}

// --- DRAWING ---

void GameEngine::DrawTile(Vec2 pos, const Tile& tile, Engine2D& engine) {
    if (!engine.isRunning()) {
        std::cerr << "Engine is not running, cannot draw to screen.\n";
        return;
    }

    Vec3 white = {1.0f, 1.0f, 1.0f};

    switch (tile.type) {
        case TileType::Null:
            engine.drawRect(tilesize_on_screen.x, tilesize_on_screen.y, white, pos);
            break;
        case TileType::Wall:
        case TileType::Rock:
        case TileType::Unknown:
            // TODO: draw other tile types
            break;
    }
}

// --- MAIN LOOP ---

void GameEngine::StartEngine() {
    Engine2D engine(screen_width, screen_height);  // first and only engine instance
    World world;  // first and only world instance
    world.init();

   

    Player player;  // created playe instance
    player.engine = &engine; // set player pointer to init engine 
    player.world = &world;
    player.init(); // set player pointer to init world

    player.tilesize_on_screen = tilesize_on_screen;
    player.isontile = world.spawntile;
    player.player_pos_on_screen = {0.0f, 0.0f};

    Window window;
    window.world = &world; 
    window.window_sizex = *tiles_on_screenx;
    window.window_sizey = *tiles_on_screeny;

    while (engine.isRunning()) {
        engine.beginFrame();

        window.window_pos = subVec2pos(player.worldpos,
                                       Vec2{static_cast<float>(*tiles_on_screenx), static_cast<float>(*tiles_on_screeny)});

        window.chunks_in_win = window.chunks_in_window(); 
        window.tiles_in_win = window.tiles_in_window(); 

        DrawWindow(window, engine, world); 
        player.DrawPlayer(); 

        engine.endFrame(); 
    }
}

void GameEngine::DrawWindow(const Window& window, Engine2D& engine, World& world) {
    for (const auto& tile : window.tiles_in_win.tiles) {
        int world_x = static_cast<int>(tile.chunk_pos.x) * world.tiles_per_chunk + tile.inside_chunk_pos.x;
        int world_y = static_cast<int>(tile.chunk_pos.y) * world.tiles_per_chunk + tile.inside_chunk_pos.y;

        Vec2 tile_screen_pos = {
            (world_x - window.window_pos.x) * tilesize_on_screen.x,
            (world_y - window.window_pos.y) * tilesize_on_screen.y
        };

        DrawTile(tile_screen_pos, tile, engine);
    }
}

void GameEngine::init() {
    tilesize_on_screen = {

        2.0f / static_cast<float>(*tiles_on_screenx),
        2.0f / static_cast<float>(*tiles_on_screeny)

    };

}