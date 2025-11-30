
#include "game_engine.hpp"
#include <iostream>

// --- PLAYER ---

void GameEngine::Player::move_player(Vec2 amount, World& world) {
    int world_posx = static_cast<int>(getworldcords(isontile.inside_chunk_pos, isontile.chunk_pos, world.tiles_per_chunk).x); // number between 0, tiles_per_chunk*chunks_per_worldx (80)
    int world_posy = static_cast<int>(getworldcords(isontile.inside_chunk_pos, isontile.chunk_pos, world.tiles_per_chunk).y); // number between 0, tiles_per_chunk*chunks_per_worldx (80)

    // grab the tile directly (assuming you’ve got something like GetTileGlobal or GetTileAt)
    isontile = world.GetTileGlobal(
        static_cast<int>(world_posx),
        static_cast<int>(world_posy)
    );

    
}

void GameEngine::Player::DrawPlayer(Vec2 tos) { // tos stands for tilesize on screen
    Vec3 player_color = {1.0f, 0.0f, 0.0f}; // red

    if (!engine || !engine->isRunning()) {
        std::cerr << "Engine not running, cannot draw player\n";
        return;
    }

    engine->drawRect(
        tos.x,
        tos.y,
        player_color,
        player_pos_on_screen
    );
}

void GameEngine::Player::init() {
    // Validate all required pointers are set
    if (!engine) {
        std::cerr << "ERROR GameEngine::Player::init(): engine pointer not set!\n";
        std::exit(1);
    }
    if (!world) {
        std::cerr << "ERROR GameEngine::Player::init(): world pointer not set!\n";
        std::exit(1);
    }

    // Removed incorrect tilesize calculation (was using engine pixel dims * tiles_per_chunk).
    // Player will use GameEngine::tilesize_on_screen when drawing.
    isontile = world->spawntile;
    player_pos_on_screen = {0.0f, 0.0f};
}

// --- WINDOW ---

Chunks GameEngine::Window::chunks_in_window() {
    int win_startx = static_cast<int>(window_pos.x);
    int win_starty = static_cast<int>(window_pos.y);
    int win_endx = win_startx + window_sizex;
    int win_endy = win_starty + window_sizey;

    Chunks chunks_in_win;

    if (!world) return chunks_in_win;

    // Corrected chunk index calculation: divide by tiles_per_chunk only.
    int cx_start = std::max(0, win_startx / world->tiles_per_chunk);
    int cy_start = std::max(0, win_starty / world->tiles_per_chunk);
    int cx_end = std::min(world->chunks_per_worldx - 1, win_endx / world->tiles_per_chunk);
    int cy_end = std::min(world->chunks_per_worldy - 1, win_endy / world->tiles_per_chunk);

    for (int cx = cx_start; cx <= cx_end; cx++) {
        for (int cy = cy_start; cy <= cy_end; cy++) {
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

// --- ENGINE ---

void GameEngine::DrawTile(Vec2 pos, const Tile& tile) {
    if (!engine->isRunning()) {
        std::cerr << "Engine is not running, cannot draw to screen.\n";
        return;
    }

    Vec3 null_black = {0.0f, 0.0f, 0.0f};
    Vec3 wall_brown = {1.0f, 0.7f, 0.5f};
    Vec3 room_grey = {0.5f, 0.5f, 0.5f};
    Vec3 unknown_white = {1.0f, 1.0f, 1.0f};
    Vec3 room_darkgrey = {0.8f, 0.8f, 0.8f};


    switch (tile.type) {
        case TileType::Null:
            engine->drawRect(tilesize_on_screen.x, tilesize_on_screen.y, null_black, pos);
            break;
        case TileType::Wall:
            engine->drawRect(tilesize_on_screen.x, tilesize_on_screen.y, wall_brown, pos);
            break;
        case TileType::Rock:
        case TileType::Room:
            engine->drawRect(tilesize_on_screen.x, tilesize_on_screen.y, room_grey, pos);
            break;
        case TileType::RoomEdge:
            engine->drawRect(tilesize_on_screen.x, tilesize_on_screen.y, room_darkgrey, pos);
            break;
        case TileType::Unknown:
            engine->drawRect(tilesize_on_screen.x, tilesize_on_screen.y, unknown_white, pos);
            break;
    }
}

void GameEngine::StartEngine() {
    fs::path path_world_on_disk = "../world";

    Player player;  // created player instance
    player.engine = engine; // set player pointer to init engine 
    player.world = world;
    player.init(); // set player pointer to init world

    Window window;
    window.world = world; 
    window.window_sizex = *tiles_on_screenx;
    window.window_sizey = *tiles_on_screeny;

    while (engine->isRunning()) {
        engine->beginFrame();

        window.window_pos = subVec2pos(Vec2{
                                  getworldcords(player.isontile.inside_chunk_pos, player.isontile.chunk_pos, world->tiles_per_chunk).x,
                                  getworldcords(player.isontile.inside_chunk_pos, player.isontile.chunk_pos, world->tiles_per_chunk).y},

                                  Vec2{static_cast<float>(*tiles_on_screenx) / 2, static_cast<float>(*tiles_on_screeny) / 2});

        window.chunks_in_win = window.chunks_in_window(); 
        window.tiles_in_win = window.tiles_in_window();

        if (texurized && *texurized) {
            DrawtexWindow(window);
        } else {
            DrawWindow(window);
        }
        
        // draw player using GameEngine::tilesize_on_screen (correct tile-size in NDC)
        engine->drawRect(
            tilesize_on_screen.x,
            tilesize_on_screen.y,
            Vec3{1.0f, 0.0f, 0.0f},
            Vec2{0.0f, 0.0f}
        );

        engine->endFrame();
    }
}

void GameEngine::DrawWindow(const Window& window) {
    for (const auto& tile : window.tiles_in_win.tiles) {
        int world_x = static_cast<int>(tile.chunk_pos.x) * world->tiles_per_chunk + static_cast<int>(tile.inside_chunk_pos.x);
        int world_y = static_cast<int>(tile.chunk_pos.y) * world->tiles_per_chunk + static_cast<int>(tile.inside_chunk_pos.y);

        // Map to normalized device coords (-1 .. 1).
        Vec2 tile_screen_pos = {
            (world_x - window.window_pos.x) * tilesize_on_screen.x - 1.0f,
            (world_y - window.window_pos.y) * tilesize_on_screen.y - 1.0f
        };

        DrawTile(tile_screen_pos, tile);
    }
}

void GameEngine::DrawtexWindow(const Window& window) {
    // Batch rendering: collect all tile geometry, then draw once
    std::vector<float> batch_vertices;

    for (const auto& tile : window.tiles_in_win.tiles) {
        int world_x = static_cast<int>(tile.chunk_pos.x) * world->tiles_per_chunk + static_cast<int>(tile.inside_chunk_pos.x);
        int world_y = static_cast<int>(tile.chunk_pos.y) * world->tiles_per_chunk + static_cast<int>(tile.inside_chunk_pos.y);

        // compute tile_screen_pos (bottom-left in NDC)
        Vec2 tile_screen_pos = {
            (world_x - window.window_pos.x) * tilesize_on_screen.x - 1.0f,
            (world_y - window.window_pos.y) * tilesize_on_screen.y - 1.0f
        };

        // Resolve texture path for this tile
        fs::path texpath = TexPathToPath(TileTypeToTexPath(tile.type));
        if (texpath.empty()) continue;

        // Load texture (from cache if available)
        if (!texmng) continue; // no texture manager available
        Texture tex = texmng->Loadtex(texpath);
        if (tex.vertices.empty()) continue; // nothing to draw

        // tex.vertices stores local UVs (u,v) and color per vertex: u,v,r,g,b
        // Map UV coordinates across the entire tile area (UV in [0,1] maps to tile size).
        // Using per-pixel scaling caused extremely small geometry because UVs are already
        // normalized. Restore mapping to full tile space so each texture fills one tile.
        const std::vector<float>& local = tex.vertices;
        for (size_t i = 0; i + 4 < local.size(); i += 5) {
            float u = local[i + 0];
            float v = local[i + 1];
            float r = local[i + 2];
            float g = local[i + 3];
            float b = local[i + 4];

            // transform local UV -> NDC within this tile rectangle (full-tile mapping)
            float x_ndc = tile_screen_pos.x + u * tilesize_on_screen.x;
            float y_ndc = tile_screen_pos.y + v * tilesize_on_screen.y;

            batch_vertices.push_back(x_ndc);
            batch_vertices.push_back(y_ndc);
            batch_vertices.push_back(r);
            batch_vertices.push_back(g);
            batch_vertices.push_back(b);
        }
    }

    // Draw all collected vertices in one call
    if (!batch_vertices.empty()) {
        engine->drawBatch(batch_vertices);
    }
}

Vec3 GameEngine::getTileColor(TileType type) {
    switch (type) {
        case TileType::Null:
            return {0.0f, 0.0f, 0.0f};      // black
        case TileType::Wall:
            return {1.0f, 0.7f, 0.5f};      // brown
        case TileType::Rock:
        case TileType::Room:
            return {0.5f, 0.5f, 0.5f};      // grey
        case TileType::Unknown:
        default:
            return {1.0f, 1.0f, 1.0f};      // white
    }
}

void GameEngine::init() {
    // Validate all required pointers are set
    if (!engine) {
        std::cerr << "ERROR GameEngine::init(): engine pointer not set!\n";
        std::exit(1);
    }
    if (!world) {
        std::cerr << "ERROR GameEngine::init(): world pointer not set!\n";
        std::exit(1);
    }
    if (!fps) {
        std::cerr << "ERROR GameEngine::init(): fps pointer not set!\n";
        std::exit(1);
    }
    if (!tiles_on_screenx) {
        std::cerr << "ERROR GameEngine::init(): tiles_on_screenx pointer not set!\n";
        std::exit(1);
    }
    if (!tiles_on_screeny) {
        std::cerr << "ERROR GameEngine::init(): tiles_on_screeny pointer not set!\n";
        std::exit(1);
    }
    if (!texurized) {
        std::cerr << "ERROR GameEngine::init(): texurized pointer not set!\n";
        std::exit(1);
    }
    if (!texmng) {
        std::cerr << "ERROR GameEngine::init(): texmng pointer not set!\n";
        std::exit(1);
    }

    tilesize_on_screen = {
        2.0f / static_cast<float>(*tiles_on_screenx),
        2.0f / static_cast<float>(*tiles_on_screeny)
    };
}