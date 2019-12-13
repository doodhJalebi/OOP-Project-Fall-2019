#pragma once

#include "SDL_Basics.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>


class Shit{
    private:
    SDL_Renderer *mRenderer = NULL;
    SDL_Texture *poop_texture;
    SDL_Rect clipping_rect;

    static SDL_Texture* texture1;
    int m_x = 0;
    int m_y = 0;
    bool active = true; //to determine if the object needs to exist further or not.

    public:
    SDL_Rect mRect;

    Shit(SDL_Renderer *renderer, int x, int y) {
        mRenderer = renderer;
        poop_texture = texture1;
        clipping_rect = {7, 0, 19, 32};
        m_x = x - 10;
        m_y = y;

        mRect = {m_x, m_y, 19, 32};
    }



    int update() {
        if (active) {
            m_y += 10;    
            if (m_y <= 0 || m_y >= SCREEN_HEIGHT) {
                active = false;
                return 1;
            } else {
                mRect.y = m_y;
                draw();
            }
            return 0;
        } else {
            return 1;
        }
    }

    void draw() {
        //SDL_SetRenderDrawColor( mRenderer, 0, 0, 255, 0xFF );
        //SDL_RenderFillRect( mRenderer, &mRect );

        SDL_RenderCopy( mRenderer, poop_texture, &clipping_rect, &mRect);
    }

    static void loadSprites() {
        texture1 = loadTexture("Sprites/poop.png");
    }
};

SDL_Texture *Shit::texture1 = nullptr;