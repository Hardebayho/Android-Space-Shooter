#include "Explosion.h"
#include <cmath>

#include <allegro5/allegro_primitives.h>

Explosion::Explosion(float time, ALLEGRO_COLOR color_, float x, float y) : color(color_) {
    for (int i = 0; i < 200; i++) {
        float angle = (rand() % 361) * (3.14159f / 180);
        float speed = 2 + rand() % 6;
        Particle particle;
        particle.x = x;
        particle.y = y;
        particle.dx = cosf(angle) * speed;
        particle.dy = sinf(angle) * speed;
        
        particles.emplace_back(particle);
    }
    
    remove = false;
    explosion_queue = al_create_event_queue();
    assert(explosion_queue);
    explosion_timer = al_create_timer(time);
    assert(explosion_timer);
    al_register_event_source(explosion_queue, al_get_timer_event_source(explosion_timer));
    
    al_start_timer(explosion_timer);
}

void Explosion::update() {
    
    ALLEGRO_EVENT event;
    if (al_get_next_event(explosion_queue, &event)) {
        remove = true;
    }
    
    for (auto& particle : particles) {
        particle.x += particle.dx;
        particle.y += particle.dy;
    }
}

void Explosion::render() {
    for (auto& particle : particles)
        al_draw_circle(particle.x, particle.y, 1, color, 2);
}

void Explosion::pause() {
    al_flush_event_queue(explosion_queue);
    al_pause_event_queue(explosion_queue, true);
}

void Explosion::resume() {
    al_pause_event_queue(explosion_queue, false);
}

void Explosion::dispose() {
    particles.clear();
    al_destroy_event_queue(explosion_queue);
    al_destroy_timer(explosion_timer);
}
