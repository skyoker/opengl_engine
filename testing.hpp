#pragma once

#include "engine.hpp"
#include "gen_world.hpp"
#include "world.hpp"

#include <iostream>


#define SCREEN_HEIGHT 1000  // 1000 is set in bash so keep
#define SCREEN_WIDTH 1000  // 1000 is set in bash so keep


struct Testing {

    Testing();
    void Testdrawtriangle();
    void Testdrawrect();
    void Testdrawcircle();
    void Testgenworld();
    void TestLoadChunk();
};
