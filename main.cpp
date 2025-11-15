#include "game_engine.hpp"
#include "gen_world.hpp"

int main() {
    // Initialize the game world
    int FPS = 60;
    int TILES_ON_SCREEN_X = 20;
    int TILES_ON_SCREEN_Y = 20;

    GenWorld genworld; // does not require init params
    genworld.generate_world(); // generates the game world on disk 

    GameEngine gameengine;
    gameengine.screen_width = 1000;
    gameengine.screen_height = 1000;
    gameengine.fps = &FPS;
    gameengine.tiles_on_screenx = &TILES_ON_SCREEN_X;
    gameengine.tiles_on_screeny = &TILES_ON_SCREEN_Y;
    gameengine.init();

    gameengine.StartEngine();

    return 0;
}
