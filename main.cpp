#include "engine.hpp"

int main() {


    Engine2D engine(2000, 1000);

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

    return 0;
}
