#include <iostream>
#include "Game.h"

#include "SplashState.h"

using namespace std;

int main(int argc, char** argv) {
	Game game; // This initializes the game
	game.changeState(new SplashState(game));
	game.startGame(); // This starts the game
    return 0;
}
