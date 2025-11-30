#include "game_engine.hpp"
#include "gen_world.hpp"

int main() {
    // Initialize the game world
    int FPS = 60;
    int TILES_ON_SCREEN_X = 80;
    int TILES_ON_SCREEN_Y = 80;
    /*
        16 * 16 tiles is 62 per row
        32 * 32 tiles is 31 per row
        64 * 64 tiles is 15 per row
    */
    int SCREEN_W = 1000;
    int SCREEN_H = 1000;
    bool make_fresh_world = true;
    bool texturized = false;
    
    fs::path PATH_TO_WORLD_ON_DISK = "../world";


    GenWorld genworld;
    Engine2D engine(SCREEN_W, SCREEN_H);
    Cache cache;
    World world;
    GameEngine gameengine;
    TextureMng texmng;


    // -- genworld --
    if (make_fresh_world) {
        genworld.max_room_size = new int(10);
        genworld.min_room_size = new int(7);
        genworld.rooms_amount = new int(0);
        genworld.room_border = new int(2);
        genworld.world_border = new int(3);
        genworld.max_room_attempts = new int(5000);
        genworld.seed = new int(0);
        genworld.max_rooms_in_cluster = new int(5);
        genworld.min_rooms_in_cluster = new int(2);
        genworld.clusters = new int(3);



        genworld.gen_world();
    }
    

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
    gameengine.screen_width = SCREEN_W;
    gameengine.screen_height = SCREEN_H;
    gameengine.fps = &FPS;
    gameengine.tiles_on_screenx = &TILES_ON_SCREEN_X;
    gameengine.tiles_on_screeny = &TILES_ON_SCREEN_Y;
    gameengine.engine = &engine;
    gameengine.texmng = &texmng;
    gameengine.texurized = &texturized;
    
    // -- Set cache pointer for texture manager --
    texmng.cache = &cache;
    
    gameengine.init();

    gameengine.StartEngine();

    return 0;
}
