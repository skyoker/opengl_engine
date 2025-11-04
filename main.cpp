#include "testing.hpp"

int main() {
    GenWorld genworld;
    genworld.generate_world();
    
    GameEngine gameengine;
    gameengine.screen_width = SCREEN_WIDTH;
    gameengine.screen_height = SCREEN_HEIGHT;
    gameengine.StartEngine();

    return 0;
}
