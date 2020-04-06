#include "Powerup.h"
#include "Constants.h"

Powerup::Powerup(int type_, float x_, float y_) : type(type_) {
    this->x = x_;
    this->y = y_;
    
    init();
}

void Powerup::init() {
    lives = 1;
    dead = false;
    dx = 0;
    dy = 2;
    
    switch (type) {
    case 1: // Increase power
        powerupImage = al_load_bitmap("res/more_power.png");
        break;
    case 2: // Screen swipe
        powerupImage = al_load_bitmap("res/screen_swipe.png");
        break;
    }
    assert(powerupImage);
    width = al_get_bitmap_width(powerupImage);
    height = al_get_bitmap_height(powerupImage);
}

void Powerup::update() {
    x += dx;
    y += dy;
    
    dead = y > GAME_HEIGHT;
}

void Powerup::render() {
    al_draw_bitmap(powerupImage, x, y, 0);
}

void Powerup::dispose() {
    al_destroy_bitmap(powerupImage);
}
