#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <allegro5/allegro_audio.h>
#include "State.h"
#include "Button.h"


class MenuState : public State {
public:
    MenuState(Game& game);
    void init() override;
    void update() override;
    void handleInput(ALLEGRO_EVENT& event) override;
    void render() override;
    void dispose() override;
    void pause() override;
    void resume() override;
        
private:
    ALLEGRO_BITMAP* menu_bg;
    Button* playButton;
    Button* quitButton;

    ALLEGRO_AUDIO_STREAM* backgroundMusic;
};

#endif // MENUSTATE_H
