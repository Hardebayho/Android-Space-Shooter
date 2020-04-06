/*
 * Copyright (c) 2020 Adebayo Jagunmolu <smallwondertechnologies@gmail.com>
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

#include "LevelSelectState.h"
#include "Game.h"
#include "Constants.h"

#include "PlayState.h"

#include <iostream>
#include <string>

LevelSelectState::LevelSelectState(Game& game) : State(game) {}

void LevelSelectState::init() {
    titleFont = al_load_font("res/titleFont.ttf", 40, 0);
    assert(titleFont);
    
    // Algorithm for setting the horizontal positions of the buttons
    // i greater than 0 ? 40 : 20 + (i * image_width)
    
    // Initialize all the buttons for the levels
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 4; col++) {
            Button* button = new Button("res/item_image.png", (50 + col * 100) - 75 / 2, row * 120 + 100);
            button->setText(std::to_string((row * 4) + col + 1));
            levelSelectButtons.emplace_back(button);
        }
    }
}

void LevelSelectState::update() {
    
    for (auto& button : levelSelectButtons) {
        if (button->isClicked()) {
            std::string buttonText = button->getText();
            int levelNumber = std::stoi(buttonText.c_str());
            game.changeState(new PlayState(game, levelNumber));
            
            break;
        }
    }
    
}

void LevelSelectState::render() {
    al_draw_textf(titleFont, al_map_rgb_f(0, 1, 0), GAME_WIDTH / 2, 30, ALLEGRO_ALIGN_CENTER, "%s", "Level Select");
    
    for (auto& button : levelSelectButtons) {
        button->render();
    }
}

void LevelSelectState::handleInput ( ALLEGRO_EVENT& event ) {
    for (auto& button : levelSelectButtons) {
        button->update(event);
    }
}

void LevelSelectState::dispose() {
    al_destroy_font(titleFont);
    titleFont = nullptr;
    
    for (auto& button : levelSelectButtons) {
        button->dispose();
        delete button;
    }
}




