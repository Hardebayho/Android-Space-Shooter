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

#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include <iostream>

#include "Utils.h"

/**
 * A clickable game button
 */
class Button
{
public:
    /**
     * Constructs a new Button instance
     */
    Button (std::string imagePath, float _x, float _y);
    
    /// Initializes the button.
    void init();
    
    /// Updates the current state of the button.
    void update(ALLEGRO_EVENT& event);
    
    /// Draws the button to the screen
    void render();
    
    /// Deallocates the button image
    void dispose();
    
    /// Tells whether the button has been clicked
    bool isClicked() { return clicked; }
    
    /// Tells whether the button is held down
    bool isDown() { return down; }
    
    /// Resets the button's state
    void reset() {
        clicked = false;
        down = false;
    }
    
    void setX(float _x) { x = _x; }
    void setY(float _y) { y = _y; }
    
    float getX() { return x; }
    float getY() { return y; }
    
    float getWidth() { return width; }
    float getHeight() { return height; }
    
    /// Sets the button's text
    void setText(std::string text_) {
        text = text_;
        
        if (textFont) 
            al_destroy_font(textFont);
        
        textFont = al_load_font("res/titleFont.ttf", height * 0.4, 0);
        
        // Font dimensions
        int fx, fy, fw, fh;
        al_get_text_dimensions(textFont, text.c_str(), &fx, &fy, &fw, &fh);
        
        // Calculate the final position of the text and remove the offset
        text_x = (x + width/ 2 - fw / 2) - fx;
        text_y = (y + height/ 2 - fh / 2) - fy;
    }
    
    std::string getText() { return text; }
    
private:
    ALLEGRO_BITMAP* button_image;
    ALLEGRO_MOUSE_STATE mouse_state;
    int mouse_x;
    int mouse_y;
    float x;
    float y;
    float width;
    float height;
    bool clicked;
    bool down;
    
    std::string text;
    ALLEGRO_FONT* textFont;
    float text_x;
    float text_y;
};

#endif // BUTTON_H
