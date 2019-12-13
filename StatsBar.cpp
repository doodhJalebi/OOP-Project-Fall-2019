#pragma once

#include "SDL_Basics.cpp"

#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>

class StatsBar {
    private:
    //SDL objects for the bouding boxes.
    SDL_Rect background_box;
    SDL_Rect inner_box;
    //SDL objects for each of the bars.
    SDL_Rect stamina_bar;
    SDL_Rect time_bar;

    SDL_Renderer *mRenderer;

    //Font declarations.
    TTF_Font *score_font;
    TTF_Font *regular_font;
    SDL_Color font_color;

    //Surface, texture, and rectangle declarations for use by our different bars.
    SDL_Surface* text_surface_score;
    SDL_Texture *text_texture_score;
    SDL_Rect text_rect_score;
    int text_width_score;
    int text_height_score;

    SDL_Surface *highscore_surface;
    SDL_Texture *highscore_texture;
    SDL_Rect highscore_rect;
    int highscore_width;
    int highscore_height;

    SDL_Surface* text_surface_stamina;
    SDL_Texture *text_texture_stamina;
    SDL_Rect text_rect_stamina;
    int text_width_stamina;
    int text_height_stamina;

    SDL_Surface* text_surface_time;
    SDL_Texture *text_texture_time;
    SDL_Rect text_rect_time;
    int text_width_time;
    int text_height_time;

    int background_box_width = SCREEN_WIDTH * 0.6;
    int background_box_height = SCREEN_HEIGHT * 0.085;
    
    int inner_box_width;
    int inner_box_height;

    int stamina_bar_width;
    int time_bar_width;
    int total_time;
    int highscore;
    int old_score = 0;

    std::string score_string = "Score: 00";
    std::string highscore_string = "Best: ";

    public:

    StatsBar(SDL_Renderer *renderer, int total_time, int highscore) {

        this->total_time = total_time;
        mRenderer = renderer;
        this->highscore = highscore;


        TTF_Init();

        font_color = {255, 255, 255};
        score_font = TTF_OpenFont("Fonts/Bradley_Hand_ITC_TT_Bold.ttf", 22);
        text_surface_score = TTF_RenderText_Solid(score_font, score_string.c_str(), font_color);
        text_texture_score = SDL_CreateTextureFromSurface(mRenderer, text_surface_score);
        TTF_SizeText(score_font, score_string.c_str(), &text_width_score, &text_height_score);

        highscore_string = "Best: " + std::to_string(highscore);
        highscore_surface = TTF_RenderText_Solid(score_font, highscore_string.c_str(), font_color);
        highscore_texture = SDL_CreateTextureFromSurface(mRenderer, highscore_surface);
        TTF_SizeText(score_font, highscore_string.c_str(), &highscore_width, &highscore_height);


        regular_font = TTF_OpenFont("Fonts/Bradley_Hand_ITC_TT_Bold.ttf", 22);
        text_surface_stamina = TTF_RenderText_Solid(regular_font, "Stamina:", font_color);
        text_texture_stamina = SDL_CreateTextureFromSurface(mRenderer, text_surface_stamina);
        TTF_SizeText(regular_font, "Stamina:", &text_width_stamina, &text_height_stamina);

        text_surface_time = TTF_RenderText_Solid(regular_font, "Time:", font_color);
        text_texture_time = SDL_CreateTextureFromSurface(mRenderer, text_surface_time);
        TTF_SizeText(regular_font, "Time:", &text_width_time, &text_height_time);
        
        text_rect_score = {background_box_width - text_width_score, 15, text_width_score, text_height_score};

        text_rect_stamina = {17, 15, text_width_stamina, text_height_stamina};

        text_rect_time = {17, 40, text_width_time, text_height_time};

        highscore_rect = {background_box_width - highscore_width, 40, highscore_width, highscore_height};



        background_box = {10, 10, background_box_width, background_box_height};

        inner_box_width = background_box_width - 10;
        inner_box_height = background_box_height - 10;

        inner_box = {15, 15, inner_box_width, inner_box_height};

    }

    

    void draw(float stamina, double time, int score) {
        SDL_SetRenderDrawColor( mRenderer, 102, 102, 102, 0xFF );
        SDL_RenderFillRect(mRenderer, &background_box);

        SDL_SetRenderDrawColor( mRenderer, 255, 255, 255, 0xFF );
        SDL_RenderDrawRect(mRenderer, &inner_box);

        stamina_bar_width = stamina / 100 * (inner_box_width - text_width_score - 120);
        stamina_bar = {120, 25, stamina_bar_width, 10};
        SDL_SetRenderDrawColor( mRenderer, 0, 255, 0, 0xFF );
        SDL_RenderFillRect(mRenderer, &stamina_bar);

        time_bar_width = (total_time - time) / total_time * (inner_box_width - text_width_score - 120);
        time_bar = {120, 50, time_bar_width, 10};
        SDL_SetRenderDrawColor( mRenderer, 0, 0, 255, 0xFF );
        SDL_RenderFillRect(mRenderer, &time_bar);

        if (score != old_score) {
            score_string = "Score: " + std::to_string(score);
            text_surface_score = TTF_RenderText_Solid(score_font, score_string.c_str(), font_color);
            text_texture_score = SDL_CreateTextureFromSurface(mRenderer, text_surface_score);
            old_score = score;
        }

        SDL_RenderCopy(mRenderer, text_texture_score, NULL, &text_rect_score);
        SDL_RenderCopy(mRenderer, highscore_texture, NULL, &highscore_rect);
        SDL_RenderCopy(mRenderer, text_texture_time, NULL, &text_rect_time);
        SDL_RenderCopy(mRenderer, text_texture_stamina, NULL, &text_rect_stamina);
    }

    void update(float stamina, double time, int score) {
        if(stamina < 0) stamina = 0;
        draw(stamina, time, score);
    }
};