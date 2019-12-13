#pragma once

#include "SDL_Basics.cpp"
#include "Bird.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>

class Sparrow : public Bird{
    private:
    SDL_Rect clippingRect[3];


    public:

    Sparrow(SDL_Renderer *renderer) : Bird(renderer) {

        //Assigining attributes to the bird which will result in the different game play.
        shitting_speed = 12;
        damage_per_hit = 10;
        damage_per_shit = 5;
        recovery_amount = 3;

        total_clips = 3;
        current_clip = 0;

        animation_speed = 6; //the higher, the slower.
        frame_count = 0;

        m_x = 0;
        m_y = 80;
        direction = 0;
        
        y_wobble = 0;
        y_wobble_factor = 0.1;


        spriteSheet = loadTexture("Sprites/Sparrow.png");
        clippingRect[0] = {4, 97, 26, 26};
        clippingRect[1] = {34, 98, 28, 25};
        clippingRect[2] = {67, 102, 29, 26};

        mRect = {m_x, m_y, 160, 140};
    }

    void draw() {
        if(DEBUG_LOCAL) {
            SDL_SetRenderDrawColor( mRenderer, 255, 0, 0, 0xFF );
            SDL_RenderFillRect( mRenderer, &mRect );
        }

        mRect.w = clippingRect[current_clip].w * 2;
        mRect.h = clippingRect[current_clip].h * 2;

        if(direction == 0) {
            SDL_RenderCopy( mRenderer, spriteSheet, &clippingRect[current_clip], &mRect);    
        } else {
            SDL_RenderCopyEx( mRenderer, spriteSheet, &clippingRect[current_clip], &mRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        }
        
        frame_count++;
        if (frame_count >= animation_speed) {
            frame_count = 0;
            current_clip++;
        }
        if(current_clip > total_clips - 1) current_clip = 0;
    }

    
};