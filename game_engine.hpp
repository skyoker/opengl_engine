#pragma once
#include "world.hpp"

/*
the game engine handels:
1. the drawing of the current screen 
2. game loop
3. player pos and drawing


*/


struct GameEngine {
    GameEngine(const int screen_width, const int screen_height, world);



    void StartEngine();


}