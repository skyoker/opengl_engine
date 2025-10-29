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

    World world(path_to_world);
    int xpos = 0;
    int ypos = 0;

    // 8 tiles per chunk
    // 100 chunks per world
    // 10 chunks per axis

    auto chunk = world.LoadChunk(xpos, ypos);
    std::cout << "tile t5x6y is " << chunk["t5x9y"]["type"] << "\n";
    std::cout << "tile t5x6y is " << chunk["t7x1y"]["type"] << "\n";
    std::cout << "tile t6x3y is " << chunk["t6x3y"]["type"] << "\n";

}