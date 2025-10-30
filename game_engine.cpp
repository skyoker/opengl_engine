#include "game_engine.hpp"


void Player::move_player(Vec2 amount) {
    auto newpos = addVec2pos(Player::player_pos, amount);
    Player::player_pos = newpos;
}

void GameEngine::StartEngine() {


}

Chunks GameEngine::Window::chunks_in_window() {
    world.

}
