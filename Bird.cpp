#pragma once

#include "SDL_Basics.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>

bool DEBUG_LOCAL = false;

class Bird{
    protected:
    SDL_Renderer *mRenderer = NULL;
    SDL_Texture *spriteSheet = NULL;

    int total_clips;
    int current_clip;

    int animation_speed; //the higher, the slower.
    int frame_count;

    int m_x;
    int m_y;
    int direction;
    
    float y_wobble;
    float y_wobble_factor;

    int texture_width;
    int texture_height;

    public:
    int shitting_speed;
    int damage_per_hit;
    int damage_per_shit;
    int recovery_amount;

    int y_wobble_amount = 360;
    SDL_Rect mRect;

    Bird(SDL_Renderer *renderer) {
        mRenderer = renderer;
    }


    virtual void update(int y, int direction_switch) {
        y_wobble = y_wobble + y_wobble_factor;

        if (abs(y_wobble) == y_wobble_amount) {
            y_wobble_factor *= -1;
        }

        if (direction_switch == 1) {
            direction = 0;
        } 
        else if (direction_switch == -1) {
            direction = 1;
        }

        if (m_x <= 0) direction = 0;
        if (m_x >= SCREEN_WIDTH - 140) direction = 1;
        
        if (direction == 0) m_x += 5;
        if (direction == 1) m_x -= 5;

        mRect.x = m_x;
        m_y = (int)(y + (sin(y_wobble)*20));
        mRect.y = m_y;
        draw();
    }

    virtual void draw() {
    
    }
};