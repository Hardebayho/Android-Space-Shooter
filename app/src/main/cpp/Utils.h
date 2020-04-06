//
// Created by smallwondertech on 3/28/20.
//

#ifndef SPACE_SHOOTER_DELUXE_UTILS_H
#define SPACE_SHOOTER_DELUXE_UTILS_H
#include "Constants.h"

#include <allegro5/allegro.h>

inline void transformCoordinates(int* x, int* y) {
    int dw = al_get_display_width(al_get_current_display());
    int dh = al_get_display_height(al_get_current_display());

    float scaleX = dw / (float) GAME_WIDTH;
    float scaleY = dh / (float) GAME_HEIGHT;

    int tx = *x;
    int ty = *y;

    if (dw >= GAME_WIDTH) { // The window is larger than our game world
        *x = static_cast<int>(tx / (scaleX >= 1 ? scaleX : 1)); // We make sure to not divide by zero
    } else {
        float temp = ((float) GAME_WIDTH / dw); // Invert the division
        *x = static_cast<int>(tx * (temp >= 1 ? temp : 1));
    }

    if (dh >= GAME_HEIGHT) {
        *y = static_cast<int>(ty / (scaleY >= 1 ? scaleY : 1));
    } else {
        float temp = ((float)GAME_HEIGHT / dh);
        *y = static_cast<int>(ty * (temp >= 1 ? temp : 1));
    }
}


#endif //SPACE_SHOOTER_DELUXE_UTILS_H
