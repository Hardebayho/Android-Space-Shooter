#include "Player.h"
#include "Constants.h"
#include <iostream>

#include "Utils.h"

Player::Player() {}

void Player::init() {
    
    ship = al_load_bitmap("res/player.png");
    
    width = al_get_bitmap_width(ship);
    height = al_get_bitmap_height(ship);
    
    x = GAME_WIDTH / 2 - width / 2;
    y = GAME_HEIGHT - height * 2;
    
    dx = 0;
    dy = 0;
    
    dead = false;
    lives = 3;
    
    mouse_down = false;
    mouse_x = 0;
    mouse_y = 0;
    
    // Fire 5 times every second
    bulletsFiringTimer = al_create_timer(1.0/5);
    bulletsFiringQueue = al_create_event_queue();
    al_register_event_source(bulletsFiringQueue, al_get_timer_event_source(bulletsFiringTimer));
    
    bulletShotSound = al_load_sample("res/laser2.ogg");
    currentBulletDamage = 1;
}

void Player::update() {
    
    if (!al_get_timer_started(bulletsFiringTimer)) al_start_timer(bulletsFiringTimer);
    
    if (al_get_next_event(bulletsFiringQueue, &bulletsFiringEvent)) {
        // Add a new bullet to the list
        Bullet* bullet = new Bullet(currentBulletDamage, "res/player_enhanced_bullet.png", -90, x + 10, y);
        
        ALLEGRO_COLOR bulletColor;
        
        switch (currentBulletDamage) {
        case 1:
            bulletColor = al_map_rgb_f(1, 0, 1);
            break;
        case 2:
            bulletColor = al_map_rgb_f(1, 1, 0);
            break;
        case 3:
            bulletColor = al_map_rgb_f(0, 1, 1);
            break;
        case 4:
            bulletColor = al_map_rgb_f(1, 1, 1);
            break;
        case 5:
            bulletColor = al_map_rgb_f(0, 1, 0);
            break;
        }
        
        // Set bullet color based on the current bullet damage
        bullet->setColor(bulletColor);
        
        bullets.emplace_back(bullet);
        
        bullet = new Bullet(currentBulletDamage, "res/player_enhanced_bullet.png", -90, x + 55, y);
        
        // Set bullet color based on the current bullet damage
        bullet->setColor(bulletColor);
        
        bullets.emplace_back(bullet);
        
        // Play the bullet fire sound
        al_play_sample(bulletShotSound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, &bulletShotSoundID);
    }
    
    if (mouse_down) {
        x = mouse_x - width / 2;
        y = mouse_y - height / 2;
    }
    
    // Update the bullets
    for (auto& bullet : bullets) {
        bullet->update();
    }
    
    // Check for removable bullets
    for (auto bulletIter = bullets.begin(); bulletIter != bullets.end(); bulletIter++) {
        if ((*bulletIter)->isDead()) {
            bullets.erase(bulletIter);
            bulletIter--;
        }
    }
}

void Player::handleInput(ALLEGRO_EVENT &event) {

#ifndef ALLEGRO_ANDROID
    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }
    
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (isInside(mouse_x, mouse_y)) {
            mouse_down = true;
        }
    }
    
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        mouse_down = false;
    }
#else
    if (event.type == ALLEGRO_EVENT_TOUCH_MOVE && event.touch.id == 0 && mouse_down) {
        mouse_x = (int) event.touch.x;
        mouse_y = (int) event.touch.y;
        transformCoordinates(&mouse_x, &mouse_y);
    }

    if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN && event.touch.id == 0) {

        mouse_x = (int) event.touch.x;
        mouse_y = (int) event.touch.y;
        transformCoordinates(&mouse_x, &mouse_y);

        if (isInside(mouse_x, mouse_y)) {
            mouse_down = true;
        }
    }

    if (event.type == ALLEGRO_EVENT_TOUCH_END && mouse_down) {
        mouse_down = false;
    }

#endif
    
}

void Player::pause() {
    al_flush_event_queue(bulletsFiringQueue);
    al_pause_event_queue(bulletsFiringQueue, true);
}

void Player::resume() {
    al_pause_event_queue(bulletsFiringQueue, false);
}

void Player::render() {
    al_draw_bitmap(ship, x, y, 0);
    for (auto& bullet : bullets) {
        bullet->render();
    }
}

void Player::dispose() {
    al_destroy_bitmap(ship);
    ship = nullptr;
    
    al_destroy_event_queue(bulletsFiringQueue);
    bulletsFiringQueue = nullptr;
    al_destroy_timer(bulletsFiringTimer);
    bulletsFiringTimer = nullptr;
    
    for (auto& bullet : bullets) {
        bullet->dispose();
        delete bullet;
    }
    
    bullets.clear();
    
    al_destroy_sample(bulletShotSound);
}
