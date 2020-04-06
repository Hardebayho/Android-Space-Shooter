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

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <allegro5/allegro.h>

/// This class is the base class for all objects in the game.
class GameObject {
public:
    /// Game Object Base Class Constructor. Does nothing for now
    GameObject();
    
    /// Initializes the fields in the object
    virtual void init() = 0;
    
    /// Updates the game object's logic
    virtual void update() = 0;
    
    /// Handles game input (if needed)
    virtual void handleInput(ALLEGRO_EVENT& event);
    
    /// Draws the game object to the screen
    virtual void render() = 0;
    
    /// Disposes of the game object
    virtual void dispose() = 0;
    
    /// Pauses some components of the game object
    virtual void pause();
    
    /// Resumes the paused components of the game object
    virtual void resume();
    
    void setX(float _x) { x = _x; }
    void setY(float _y) { y = _y; }
    void setWidth(float _width) { width = _width; }
    void setHeight(float _height) { height = _height; }
    
    void setDx(float _dx) { dx = _dx; }
    void setDy(float _dy) { dy = _dy; }
    
    float getX() { return x; }
    float getY() { return y; }
    float getWidth() { return width; }
    float getHeight() { return height; }
    
    float getDx() { return dx; }
    float getDy() { return dy; }
    
    int getLives() { return lives; }
    bool isDead() { return dead || lives <= 0; }
    
    bool intersects(GameObject* other);
    
    /// Checks whether a point is inside the object's box
    bool isInside(float px, float py) {
        return x < px && x + width > px && y < py && y + height > py;
    }
    
protected:
    // Positions
    float x;
    float y;
    
    // Dimensions
    float width;
    float height;
    
    // Vectors
    float dx;
    float dy;
    
    // Lives
    int lives;
    bool dead;

};

#endif // GAMEOBJECT_H
