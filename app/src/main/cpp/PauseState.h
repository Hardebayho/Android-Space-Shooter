#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include "State.h"
#include "Button.h"

class PauseState : public State
{
public:
    PauseState(Game& game);
    void init() override;
    void update() override;
    void handleInput(ALLEGRO_EVENT& event) override;
    void render() override;
    void dispose() override;
    
private:
    ALLEGRO_BITMAP* pause_bg;
    Button* continueButton;
    Button* quitButton;
};

#endif // PAUSESTATE_H
