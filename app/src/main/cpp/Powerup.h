#ifndef POWERUP_H
#define POWERUP_H
#include <allegro5/allegro.h>

#include "GameObject.h"

class Powerup : public GameObject
{
public:
    Powerup(int type, float x, float y);
    void init() override;
    void update() override;
    void render() override;
    void dispose() override;
    int getType() { return type; }
    
    void removeNow() { dead = true; }
    
private:
    ALLEGRO_BITMAP* powerupImage;
    int type;
};

#endif // POWERUP_H
