#include "testing.hpp"

Testing::Testing() {}


void Testing::Testdrawtriangle() {
        Engine2D engine(SCREEN_WIDTH, SCREEN_HEIGHT);
        while (engine.isRunning()) {
            engine.beginFrame();

            // draw a red triangle
            Vec2 a{ -0.5f, -0.5f };
            Vec2 b{  0.5f, -0.5f };
            Vec2 c{  0.0f,  0.5f };

            Vec3 red{ 1.0f, 0.0f, 0.0f };

            engine.drawTriangle(a, b, c, red);

            engine.endFrame();
    }

    }

void Testing::Testdrawrect() {
    Engine2D engine(SCREEN_WIDTH, SCREEN_HEIGHT); 
    while (engine.isRunning()) {
            engine.beginFrame();

            Vec2 pos = {0.0f, 0.0f}; // middle

            Vec3 red{ 1.0f, 0.0f, 0.0f }; // red

            engine.drawRect(0.5f, 0.5f, red, pos);

            engine.endFrame();
    }
}

void Testing::Testdrawcircle() {
    Engine2D engine(SCREEN_WIDTH, SCREEN_HEIGHT); 
    while (engine.isRunning()) {
            engine.beginFrame();

            Vec2 pos = {0.0f, 0.0f}; // middle

            Vec3 red{ 1.0f, 0.0f, 0.0f }; // red

            float radius = 0.6f;
            int points = 4;

            engine.drawCircle(radius, pos, points, red);

            engine.endFrame();
    }

}

void Testing::Testgenworld() {
    GenWorld genworld;
    genworld.generate_world();
}

void Testing::TestLoadChunk() {
    fs::path path_to_world = "../world";

    World world;
    world.path_to_world = &path_to_world;
    world.init();

    int xpos;
    int ypos;

    // 8 tiles per chunk
    // 100 chunks per world
    // 10 chunks per axis
    Chunk chunk;

    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            xpos = x;
            ypos = y;
            chunk = world.LoadChunk(xpos, ypos);

            std::cout << "Chunk: " << chunk.name << " at pos " << chunk.pos.x << " and " << chunk.pos.y << "\n";

        }

    }
    
}

void Testing::TestGetTile() {
    fs::path path_to_world = "../world";
    World world;
    world.path_to_world = &path_to_world;
    world.init();
    
    Chunk chunk;
    Tile tile;

    chunk = world.LoadChunk(0, 0);
    // 8 tiles per chunk
    // 100 chunks per world
    // 10 chunks per axis

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            tile = world.GetTile(x, y, chunk);

            std::cout << "Tile: " << tile.inside_chunk_pos.x << "x " << tile.inside_chunk_pos.y << "y is type " << TileTypeToString(tile.type) << "\n";

        }

    }

}

void Testing::TestDrawTile() {
    Engine2D engine(SCREEN_WIDTH, SCREEN_HEIGHT);

    // World auto-loads its info now
    World world;
    fs::path path_to_world = "../world";
    world.path_to_world = &path_to_world;
    world.init();

    // Make the GameEngine
    GameEngine gameEngine;
    gameEngine.screen_width = SCREEN_WIDTH;
    gameEngine.screen_height = SCREEN_HEIGHT;

    // Load one chunk for testing (like 0,0)
    Chunk chunk = world.LoadChunk(0, 0);

    // Main loop
    while (engine.isRunning()) {
        engine.beginFrame();

        for (const auto& tile : chunk.tiles.tiles) {
            Vec2 screenPos = {-1.0f, 0.0f};
            gameEngine.DrawTile(screenPos, tile);
        }

        engine.endFrame();
    }
}
