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

#include "Button.h"

Button::Button(std::string imagePath, float _x, float _y) : x(_x), y(_y) {
    button_image = al_load_bitmap(imagePath.c_str());
    assert(button_image);
    init();
}

void Button::init() {
    clicked = false;
    down = false;
    width = al_get_bitmap_width(button_image);
    height = al_get_bitmap_height(button_image);
    
    text = "";
    textFont = nullptr;
    text_x = 0;
    text_y = 0;
    mouse_x = 0;
    mouse_y = 0;
}

void Button::update (ALLEGRO_EVENT& event) {

#ifndef ALLEGRO_ANDROID

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;

        // Check if the cursor is inside the button
        if (mouse_x > x && mouse_x < x + width &&
            mouse_y > y && mouse_y < y + height) {
                down = true;
                clicked = true;
            }
    } else {
        clicked = false;
    }

    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        down = false;
    }
#else

    if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN && event.touch.id == 0) {
        mouse_x = (int) event.touch.x;
        mouse_y = (int) event.touch.y;

        transformCoordinates(&mouse_x, &mouse_y);

        // Check if the cursor is inside the button
        if (mouse_x > x && mouse_x < x + width &&
            mouse_y > y && mouse_y < y + height) {
            down = true;
            clicked = true;
        }
    } else {
        clicked = false;
    }

    if (event.type == ALLEGRO_EVENT_TOUCH_END && event.touch.id == 0) {
        down = false;
    }
#endif

}

void Button::render() {
    // Means clicked or down?
    bool cod = clicked || down;
    al_draw_tinted_bitmap(button_image,
                          al_map_rgb_f(cod ? 0.2f : 1, cod ? 0.2f : 1, cod ? 0.7f : 1),
                          x, y, 0);
    if (textFont) al_draw_textf(textFont, al_map_rgb_f(1, 1, 1), text_x, text_y, 0, "%s", text.c_str());
}

void Button::dispose() {
    al_destroy_bitmap(button_image);
    button_image = nullptr;
    al_destroy_font(textFont);
    textFont = nullptr;
}



