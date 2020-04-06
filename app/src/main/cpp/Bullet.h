#pragma once
#include "GameObject.h"

#include <string>
#include <allegro5/allegro_audio.h>

class Bullet : public GameObject {
    
public:
    Bullet(int damage_, std::string imagePath, float angle, float x, float y);
    void init() override;
    void update() override;
    void render() override;
    void dispose() override;
    
    float toRadians(float degrees) {
        return degrees * ALLEGRO_PI / 180;
    }
    
    float toDegrees(float radians) {
        return radians * 180 / ALLEGRO_PI;
    }
    
    /// Marks the bullet for removal
    void remove() { lives = 0; dead = true; }
    
    int getDamage() { return damage; }
    
    void setColor(ALLEGRO_COLOR color_) { color = color_; }
    void setSpeed(float value) { speed = value; }
    void setFlip(int value) { flip = value; }
    
private:
    int damage;
    ALLEGRO_BITMAP* image;
    ALLEGRO_COLOR color;
    float angle;
    float speed;
    int flip;
    
};
