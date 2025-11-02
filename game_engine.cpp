#include "game_engine.hpp"
    

void GameEngine::Player::move_player(Vec2 amount) {
    auto newpos = addVec2pos(Player::player_pos, amount);
    Player::player_pos = newpos;
}

void GameEngine::Player::DrawPlayer() {
    Vec3 player_color = {1.0f, 0.0f, 0.0f}; // red

    if (!engine.isRunning()) return; std::cerr << "window is not running cannot draw player";
    engine.drawRect(tilesize_on_screen.x, tilesize_on_screen.y, player_color, player_pos_on_screen);

}

Chunks GameEngine::Window::chunks_in_window() {
    int win_startx = static_cast<int>(window_pos.x);
    int win_starty = static_cast<int>(window_pos.y);
    int win_endx = win_startx + window_size;
    int win_endy = win_starty + window_size;

    Chunks chunks_in_win;

    // determine which chunks are in the window
    for (int cx = win_startx / (world.tiles_per_chunk * window_size);
         cx <= win_endx / (world.tiles_per_chunk * window_size); cx++) {
        for (int cy = win_starty / (world.tiles_per_chunk * window_size);
             cy <= win_endy / (world.tiles_per_chunk * window_size); cy++) {

            Chunk chunk = world.LoadChunk(cx, cy);
            chunks_in_win.add_chunk(chunk);
        }
    }

    return chunks_in_win;
}

Tiles GameEngine::Window::tiles_in_window() {
    Tiles tiles_in_win;

    for (const auto& chunk : chunks_in_win.chunks) {
        for (const auto& tile : chunk.tiles.tiles) { // access internal vector
            tiles_in_win.add_tile(tile);
        }
    }

    return tiles_in_win;
}



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
            // draw wall tile at pos
            break;

        case TileType::Rock:
            // draw rock tile at pos
            break;
        case TileType::Unknown:
            // draw unknown tile at pos
            break;
    }
}

void GameEngine::StartEngine() {
    Engine2D engine(screen_width, screen_height);
    Player player;

    while (engine.isRunning()) {
        engine.beginFrame();
        //logic goes here
        engine.endFrame();


    }
}

void GameEngine::DrawWindow(const Window& window, Engine2D& engine) {
    for (const auto& tile : window.tiles_in_win.tiles) {
        // Compute world/global tile coordinates
        int world_x = static_cast<int>(tile.chunk_pos.x) * world.tiles_per_chunk + tile.inside_chunk_pos.x;
        int world_y = static_cast<int>(tile.chunk_pos.y) * world.tiles_per_chunk + tile.inside_chunk_pos.y;

        // Convert to screen coordinates relative to window/camera
        Vec2 tile_screen_pos = {
            (world_x - window.window_pos.x) * tilesize_on_screen.x,
            (world_y - window.window_pos.y) * tilesize_on_screen.y
        };

        DrawTile(tile_screen_pos, tile, engine);
    }
}
