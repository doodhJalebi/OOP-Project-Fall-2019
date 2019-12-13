#pragma once

#include <iostream>
#include "SDL_Basics.cpp"

class Sound {
    private:
    Mix_Music *music = nullptr;
    Mix_Chunk *soundclip = nullptr;

    int type;

    public:

    Sound(std::string path, int type) { 
        //type = 0 for music, 1 for chunks.
        this->type = type;

        switch(type) {
            case 0:
            //Big sound file.
            music = Mix_LoadMUS(path.c_str());
            break;

            case 1:
            //Small sound file.
            soundclip = Mix_LoadWAV(path.c_str());
            break;
        }

        if (type == 0 && music == nullptr) {
            printf("Failed to load soundclip/music! SDL Mixer Error: %s\n", Mix_GetError());
        }
        else if (type == 1 && soundclip == nullptr) {
            printf("Failed to load soundclip/music! SDL Mixer Error: %s\n", Mix_GetError());
        }
    }

    ~Sound() {
        Mix_FreeMusic(music);
        Mix_FreeChunk(soundclip);
        delete music;
        delete soundclip;
        music = nullptr;
        soundclip = nullptr;
    }


    void play() {
        if (type == 1) {
            Mix_PlayChannel(-1, soundclip, 0);
        }
        else if (type == 0) {
            Mix_PlayMusic(music, -1);
        }
    }

    void stop() {
        Mix_HaltMusic();
    }

};