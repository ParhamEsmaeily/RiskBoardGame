
#include "GameEngine.h"
void testTournament() {
    // create new game engine
    auto *gameEngine = new GameEngine();

    // create new tournament
    gameEngine->initiateTournament();

    delete gameEngine;
    gameEngine = nullptr;
}