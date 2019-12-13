#pragma once

#include "SDL_Basics.cpp"
#include "Bird.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>

class Pigeon : public Bird{
    private:
    SDL_Rect clippingRect[4];


    public:

    Pigeon(SDL_Renderer *renderer) : Bird(renderer) {


        shitting_speed = 20;
        damage_per_hit = 15;
        damage_per_shit = 10;
        recovery_amount = 3;

        total_clips = 4;
        current_clip = 0;

        animation_speed = 6; //the higher, the slower.
        frame_count = 0;

        m_x = 0;
        m_y = 80;
        direction = 0;
        
        y_wobble = 0;
        y_wobble_factor = 0.1;


        spriteSheet = loadTexture("Sprites/Pigeons.png");
        clippingRect[0] = {1, 188, 191, 106};
        clippingRect[1] = {220, 195, 166, 72};
        clippingRect[2] = {417, 185, 164, 135};
        clippingRect[3] = {579, 185, 164, 66};

        mRect = {m_x, m_y, 160, 140};
    }

    void draw() {
        if(DEBUG_LOCAL) {
            SDL_SetRenderDrawColor( mRenderer, 255, 0, 0, 0xFF );
            SDL_RenderFillRect( mRenderer, &mRect );
        }

        mRect.w = clippingRect[current_clip].w * 0.5; //scaling our sprite.
        mRect.h = clippingRect[current_clip].h * 0.5;

        if(direction == 0) {
            SDL_RenderCopy( mRenderer, spriteSheet, &clippingRect[current_clip], &mRect);    
        } else {
            SDL_RenderCopyEx( mRenderer, spriteSheet, &clippingRect[current_clip], &mRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        }
        
        frame_count++;
        if (frame_count >= animation_speed) { //algorithm to control the speed of the animation.
            frame_count = 0;
            current_clip++;
        }
        if(current_clip > total_clips - 1) current_clip = 0;
    }

    
};