#include "MenuState.h"
#include "Game.h"
#include "Constants.h"
#include "LevelSelectState.h"

#include <iostream>

MenuState::MenuState(Game& game_) : State(game_) {}

void MenuState::init() {
    menu_bg = al_load_bitmap("res/menu.png");
    assert(menu_bg);
    
    playButton = new Button("res/play_button.png", GAME_WIDTH / 2 - (192 / 2), 440);
    quitButton = new Button("res/quit_button.png", GAME_WIDTH / 2 - (192 / 2), 565);

    backgroundMusic = al_load_audio_stream("res/menu_music.mp3", 2, 2048);
    assert(backgroundMusic);
    al_set_audio_stream_playmode(backgroundMusic, ALLEGRO_PLAYMODE_LOOP);

    al_attach_audio_stream_to_mixer(backgroundMusic, al_get_default_mixer());
}

void MenuState::update() {
    
    if (playButton->isClicked()) {
        game.changeState(new LevelSelectState(game));
    }
    
    if (quitButton->isClicked()) {
        game.quit();
    }
    
}

void MenuState::render() {
    al_draw_bitmap(menu_bg, 0, 0, 0);
    
    playButton->render();
    quitButton->render();
}

void MenuState::handleInput(ALLEGRO_EVENT &event) {
	playButton->update(event);
    quitButton->update(event);
}

void MenuState::pause() {
    al_set_audio_stream_playing(backgroundMusic, false);
}

void MenuState::resume() {
    al_set_audio_stream_playing(backgroundMusic, true);
}

void MenuState::dispose() {
    playButton->dispose();
    delete playButton;
    
    quitButton->dispose();
    delete quitButton;

    al_destroy_audio_stream(backgroundMusic);
}
