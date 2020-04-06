#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include <string>
#include <vector>

#include "Bullet.h"

class Enemy : public GameObject
{
public:
    Enemy(int rank_);
    void init() override;
    void update() override;
    void render() override;
    void dispose() override;
    
    void loseLife(int amount) { lives-= amount; }
    int getRank() { return rank; }
    
    void die() { lives = 0; dead = true; }
    
    std::vector<Bullet*> getBullets() { return bullets; }
    
private:
    ALLEGRO_BITMAP* image;
    std::string image_path;
    int rank;
    float speed;
    
    std::vector<Bullet*> bullets;
};

#endif // ENEMY_H
