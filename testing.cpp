#include "testing.hpp"

Testing::Testing() {}


void Testing::Testdrawtriangle() {
        Engine2D engine(1000, 1000); // 1000 is set in bash so keep

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
    Engine2D engine(1000, 1000); // 1000 is set in bash so keep
    while (engine.isRunning()) {
            engine.beginFrame();

            Vec2 pos = {0.0f, 0.0f}; // middle

            Vec3 red{ 1.0f, 0.0f, 0.0f }; // red

            engine.drawRect(0.5f, 0.5f, red, pos);

            engine.endFrame();
    }



}