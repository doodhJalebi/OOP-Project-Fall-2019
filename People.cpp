#pragma once

#include "SDL_Basics.cpp"


#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>

bool PEOPLE_DEBUG = false;

class People{
    private:
    SDL_Renderer *mRenderer = NULL;
    SDL_Texture *spriteSheet;
    SDL_Rect clippingRect[4][4];

    static SDL_Texture *texture1;
    static SDL_Texture *texture2;
    static SDL_Texture *texture3;
    static SDL_Texture *texture4;

    //Control variables to control the behavior of a people object.
    int total_clips = 4;
    int current_clip = 0;

    int animation_speed = 6; //the higher, the slower.
    int frame_count = 0;

    int texture_width;
    int texture_height;

    int m_x = 0;
    int direction;
    bool active = true;

    int sprite_to_choose; //randomizes between 0 and 3 to choose a sprite to render.

    float scaling_factor[4];


    public:
    int gender; //0 = male, 1 = female
    SDL_Rect mRect;

    People(SDL_Renderer *renderer) {
        mRenderer = renderer;

        srand(time(0));
        direction = rand() % 2;
        sprite_to_choose = rand() % 4;

        switch(sprite_to_choose) {
            case 0:
            spriteSheet = texture1;
            gender = 0;
            break;

            case 1:
            spriteSheet = texture2;
            gender = 0;
            break;

            case 2:
            spriteSheet = texture3;
            gender = 1;
            break;

            case 3:
            spriteSheet = texture4;
            gender = 1;
            break;
        }

        clippingRect[0][0] = {1353, 173, 765, 1753};
        clippingRect[0][1] = {2537, 173, 549, 1729};
        clippingRect[0][2] = {3593, 169, 525, 1733};
        clippingRect[0][3] = {4433, 189, 849, 1709};

        clippingRect[1][0] = {0, 0, 260, 512};
        clippingRect[1][1] = {260, 0, 255, 511};
        clippingRect[1][2] = {510, 0, 235, 512};
        clippingRect[1][3] = {747, 0, 220, 513};

        clippingRect[2][0] = {1985, 101, 541, 1865};
        clippingRect[2][1] = {2833, 109, 729, 1861};
        clippingRect[2][2] = {3737, 109, 989, 1841};
        clippingRect[2][3] = {4917, 113, 613, 1805};
        
        clippingRect[3][0] = {244, 7, 64, 127};
        clippingRect[3][1] = {356, 2, 64, 129};
        clippingRect[3][2] = {468, 0, 63, 129};
        clippingRect[3][3] = {572, 3, 64, 128};

        scaling_factor[0] = 0.11;
        scaling_factor[1] = 0.3;
        scaling_factor[2] = 0.1;
        scaling_factor[3] = 1;

        switch(direction) {
            case 0:
            //initialize at the left
            m_x = -350;
            break;

            case 1:
            //initialize at the right
            m_x = SCREEN_WIDTH;
            break;    
        }
        mRect = {m_x, SCREEN_HEIGHT - 100, 75, 100};
    }


    int update() {
        if (active) {
            switch(direction) {
            case 0:
            //moves right
            m_x += 2;
            break;

            case 1:
            //moves left
            m_x -= 2;
            break;
            }
            if (m_x <= -350 || m_x >= SCREEN_WIDTH) {
                active = false;
                return 1;
            } else {
                mRect.x = m_x;
                draw();
            }
            return 0;
        } else {
            return 1;
        }
    }

    void draw() {
        mRect.y = SCREEN_HEIGHT - (clippingRect[sprite_to_choose][current_clip].h * scaling_factor[sprite_to_choose]);
        mRect.w = clippingRect[sprite_to_choose][current_clip].w * scaling_factor[sprite_to_choose];
        mRect.h = clippingRect[sprite_to_choose][current_clip].h * scaling_factor[sprite_to_choose];


        if(PEOPLE_DEBUG) {
            SDL_SetRenderDrawColor( mRenderer, 255, 0, 0, 0xFF );
            SDL_RenderFillRect( mRenderer, &mRect );
        }
        

        
        if(direction == 0) {
            SDL_RenderCopy( mRenderer, spriteSheet, &clippingRect[sprite_to_choose][current_clip], &mRect);    
        } else {
            SDL_RenderCopyEx( mRenderer, spriteSheet, &clippingRect[sprite_to_choose][current_clip], &mRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
        }
        
        frame_count++;
        if (frame_count >= animation_speed) {
            frame_count = 0;
            current_clip++;
        }
        if(current_clip > total_clips - 1) current_clip = 0;
    
    }

    static void loadSprites() {
        texture1 = loadTexture("Sprites/walking man 2.png");
        texture2 = loadTexture("Sprites/walking boy.png");
        texture3 = loadTexture("Sprites/walking woman.png");
        texture4 = loadTexture("Sprites/walking girl.png");
    }

};


SDL_Texture* People::texture1 = nullptr;
SDL_Texture* People::texture2 = nullptr;
SDL_Texture* People::texture3 = nullptr;
SDL_Texture* People::texture4 = nullptr;