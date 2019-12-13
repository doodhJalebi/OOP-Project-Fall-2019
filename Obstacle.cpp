#pragma once

#include "SDL_Basics.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>


class Obstacle{
    private:
    SDL_Renderer *mRenderer = NULL;
    SDL_Texture *spriteSheet[2];
    SDL_Rect clippingRect[2];

    static SDL_Texture* texture1;
    static SDL_Texture* texture2;
    
    int m_x = 0;
    int m_y = 0;
    int direction; //randomizes between 0 and 1 to choose the direction for the obstacle.
    int selection; //randomizes between 0 and 1 to choose the sprite to render.
    bool active = true; //bool to determine if the object needs to exist or not.
    

    public:
    SDL_Rect mRect;

    Obstacle(SDL_Renderer *renderer) {
        mRenderer = renderer;

        srand(time(0));
        direction = rand() % 2;
        selection = rand() % 2;
        
        m_y = 100 + rand() % 200;
        m_y += (300 - m_y) * 0.5;

        spriteSheet[0] = texture1;
        clippingRect[0] = {21, 22, 110, 83};

        spriteSheet[1] = texture2;
        clippingRect[1] = {445, 201, 1057, 1493};

        switch(direction) {
            case 0:
            //initialize at the left
            m_x = -150;
            break;

            case 1:
            //initialize at the right
            m_x = SCREEN_WIDTH;
            break;    
        }

        int renderWidth0 = clippingRect[0].w * 0.5; //Scaling down our sprite.
        int renderHeight0 = clippingRect[0].h * 0.5;

        int renderWidth1 = clippingRect[1].w * 0.02;
        int renderHeight1 = clippingRect[1].h * 0.02;

        if(selection == 0) {
            mRect = {m_x, m_y, renderWidth0, renderHeight0};
        } else {
            mRect = {m_x, m_y, renderWidth1, renderHeight1};
        }
    }

    int update() {
        if (active) {
            switch(direction) {
            case 0:
            //moves right
            m_x += 4;
            break;

            case 1:
            //moves left
            m_x -= 4;
            break;
            }
            if (m_x <= -150 || m_x >= SCREEN_WIDTH) {
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
        //SDL_SetRenderDrawColor( mRenderer, r, g, b, 0xFF );
        //SDL_RenderFillRect( mRenderer, &mRect );

        if(selection == 0) {
            if(direction == 0) {
                SDL_RenderCopy( mRenderer, spriteSheet[0], &clippingRect[0], &mRect);
            } else {
                SDL_RenderCopyEx( mRenderer, spriteSheet[0], &clippingRect[0], &mRect, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
        } else {
            SDL_RenderCopy( mRenderer, spriteSheet[1], &clippingRect[1], &mRect);
        }
        
    }

    static void loadSprites() {
        texture1 = loadTexture("Sprites/paper plane.png");
        texture2 = loadTexture("Sprites/can.png");
    }

};

SDL_Texture* Obstacle::texture1 = nullptr;
SDL_Texture* Obstacle::texture2 = nullptr;