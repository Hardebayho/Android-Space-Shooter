#include "Bullet.h"
#include <cmath>
#include "Constants.h"

Bullet::Bullet(int damage_, std::string imagePath, float angle_, float _x, float _y) : damage(damage_), angle(angle_) {
    image = al_load_bitmap(imagePath.c_str());
    assert(image);
    
    this->x = _x;
    this->y = _y;
    
    init();
}

void Bullet::init() {
    speed = 10;
    lives = 1;
    width = al_get_bitmap_width(image);
    height = al_get_bitmap_height(image);
    
    dx = cosf(toRadians(angle)) * speed;
    dy = sinf(toRadians(angle)) * speed;
    
    color = al_map_rgb_f(1, 1, 1);
    flip = 0;
}

void Bullet::update() {
    x += dx;
    y += dy;
    
    if (x < 0 || x >= GAME_WIDTH + width || y < 0 || y > GAME_HEIGHT + height)
        lives--;
    
    dead = lives <= 0;
}

void Bullet::render() {
    al_draw_tinted_bitmap(image, color, x, y, flip);
}

void Bullet::dispose() {
    al_destroy_bitmap(image);
    image = nullptr;
}
