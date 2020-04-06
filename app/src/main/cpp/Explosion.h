#ifndef EXPLOSION_H
#define EXPLOSION_H
#include <vector>

#include <allegro5/allegro.h>

struct Particle {
    float x;
    float y;
    float dx;
    float dy;
};

class Explosion
{
public:
    Explosion(float time, ALLEGRO_COLOR color, float x, float y);
    void update();
    void render();
    void dispose();
    
    bool isDone() { return remove; }
    
    void pause();
    void resume();
    
private:
    std::vector<Particle> particles;
    
    ALLEGRO_TIMER* explosion_timer;
    ALLEGRO_EVENT_QUEUE* explosion_queue;
    ALLEGRO_COLOR color;
    bool remove;
};

#endif // EXPLOSION_H
