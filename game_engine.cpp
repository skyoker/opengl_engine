#include "game_engine.hpp"


void Player::move_player(Vec2 amount) {
    auto newpos = addVec2pos(Player::player_pos, amount);
    Player::player_pos = newpos;
}

void GameEngine::StartEngine() {


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



void DrawTile(Vec2 pos, const Tile& tile) {
    std::string tiletype = tile.type;
    switch tiletype;
    

}