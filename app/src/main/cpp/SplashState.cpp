#include "SplashState.h"
#include "Game.h"
#include "MenuState.h"

SplashState::SplashState(Game& game_) : State(game_) {}

void SplashState::init() {
    splash_image = al_load_bitmap("res/splash.png");
    assert(splash_image);
    
    splash_queue = al_create_event_queue();
    splash_timer = al_create_timer(6);
    al_register_event_source(splash_queue, al_get_timer_event_source(splash_timer));
    
    al_start_timer(splash_timer);
}

void SplashState::update() {
    if (al_get_next_event(splash_queue, &splash_event)) {
        game.changeState(new MenuState(game));
    }
}

void SplashState::handleInput(ALLEGRO_EVENT &event) {}

void SplashState::render() {
    al_draw_bitmap(splash_image, 0, 0, 0);
}

void SplashState::dispose() {
    al_destroy_bitmap(splash_image);
    al_destroy_event_queue(splash_queue);
    al_destroy_timer(splash_timer);
}
