#include "PauseState.h"
#include "Constants.h"
#include "Game.h"
#include "MenuState.h"

PauseState::PauseState(Game& game_) : State(game_) {}

void PauseState::init() {
    pause_bg = al_load_bitmap("res/pause_bg.png");
    assert(pause_bg);
    
    continueButton = new Button("res/continue_button.png", GAME_WIDTH / 2 - (192 / 2), 250);
    quitButton = new Button("res/quit_button.png", GAME_WIDTH / 2 - (192 / 2), 440);
}

void PauseState::update() {
    if (continueButton->isClicked()) {
        game.resumeState();
    }
    
    if (quitButton->isClicked()) {
        game.changeState(new MenuState(game));
    }
}

void PauseState::handleInput(ALLEGRO_EVENT &event) {
    continueButton->update(event);
    quitButton->update(event);
}

void PauseState::render() {
    al_draw_bitmap(pause_bg, 0, 0, 0);
    
    continueButton->render();
    quitButton->render();
}

void PauseState::dispose() {
    al_destroy_bitmap(pause_bg);
    
    continueButton->dispose();
    delete continueButton;
    
    quitButton->dispose();
    delete quitButton;
}
