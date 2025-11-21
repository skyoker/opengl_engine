#include "game_engine.hpp"
#include "gen_world.hpp"

#include "testing.hpp"

int main() {
    GenWorld genworld;
    Testing testsuite;
     // does not require init params
    testsuite.gendebugworld();

    return 0;
}
