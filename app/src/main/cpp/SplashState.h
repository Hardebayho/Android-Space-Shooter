#ifndef SPLASHSTATE_H
#define SPLASHSTATE_H
#include "State.h"

class SplashState : public State {
public:
    SplashState(Game& game);
    void init() override;
    void update() override;
    void handleInput(ALLEGRO_EVENT& event) override;
    void render() override;
    void dispose() override;
    
private:
    ALLEGRO_BITMAP* splash_image;
    ALLEGRO_EVENT_QUEUE* splash_queue;
    ALLEGRO_TIMER* splash_timer;
    ALLEGRO_EVENT splash_event;
    
};

#endif // SPLASHSTATE_H
