/*
 * Copyright (c) 2020 Adebayo Jagunmolu <email>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LEVELSELECTSTATE_H
#define LEVELSELECTSTATE_H
#include "State.h"

#include <allegro5/allegro_font.h>
#include <vector>

#include "Button.h"

/**
 * @todo This state allows to select various levels in the game.
 */
class LevelSelectState : public State
{
public:
    /**
     * Default constructor
     */
    LevelSelectState(Game& game);
    void init() override;
    void update() override;
    void render() override;
    void handleInput(ALLEGRO_EVENT& event) override;
    void dispose() override;
    
private:
    ALLEGRO_FONT* titleFont;
    ALLEGRO_FONT* itemFont;
    
    std::vector<Button*> levelSelectButtons;

};

#endif // LEVELSELECTSTATE_H
