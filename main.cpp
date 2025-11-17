#include "game_engine.hpp"
#include "gen_world.hpp"

int main() {
    // Initialize the game world
    int FPS = 60;
    int TILES_ON_SCREEN_X = 20;
    int TILES_ON_SCREEN_Y = 20;
    fs::path PATH_TO_WORLD_ON_DISK = "../world";


    GenWorld genworld; // does not require init params
    Engine2D engine;
    Cache cache;
    World world;
    GameEngine gameengine;


    // -- genworld --
    genworld.generate_world(); // generates the game world on disk 
    // -- engine --
    // nothing to do
    // -- cache --
    // nothing to do 
    // -- world --
    world.path_to_world = &PATH_TO_WORLD_ON_DISK;
    world.cache = &cache;
    world.init();
    // -- gameengine --
    gameengine.world = &world;
    gameengine.screen_width = 1000;
    gameengine.screen_height = 1000;
    gameengine.fps = &FPS;
    gameengine.tiles_on_screenx = &TILES_ON_SCREEN_X;
    gameengine.tiles_on_screeny = &TILES_ON_SCREEN_Y;
    gameengine.init();

    gameengine.StartEngine();

    return 0;
}
