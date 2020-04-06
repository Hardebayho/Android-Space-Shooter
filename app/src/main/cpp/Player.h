#pragma once
#include "GameObject.h"
#include <vector>

#include "Bullet.h"
#include <allegro5/allegro_font.h>

class Player : public GameObject {

public:
    Player();
    void init() override;
    void update() override;
    void handleInput(ALLEGRO_EVENT& event) override;
    void render() override;
    void dispose() override;
    void pause() override;
    void resume() override;
    
    std::vector<Bullet*> getBullets() { return bullets; }
    
    void increaseBulletDamage() { ++currentBulletDamage >= 5 ? currentBulletDamage = 5 : 0; }
    void setBulletDamage(int value) { currentBulletDamage = value; }
    int getBulletDamage() { return currentBulletDamage; }
    void die() { lives = 0; }
    
private:
    /// Player's ship
    ALLEGRO_BITMAP* ship;
    
    bool mouse_down;
    int mouse_x;
    int mouse_y;
    
    // Bullets
    ALLEGRO_TIMER* bulletsFiringTimer;
    ALLEGRO_EVENT_QUEUE* bulletsFiringQueue;
    ALLEGRO_EVENT bulletsFiringEvent;
    std::vector<Bullet*> bullets;
    
    ALLEGRO_SAMPLE* bulletShotSound;
    ALLEGRO_SAMPLE_ID bulletShotSoundID;
    
    ALLEGRO_FONT* numBulletsFont;
    
    int currentBulletDamage;

};
