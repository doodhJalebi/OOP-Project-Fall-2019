#pragma once
#include "SDL_Basics.cpp"

#include "People.cpp"
#include "Shit.cpp"
#include "Pigeon.cpp"
#include "Sparrow.cpp"
#include "Obstacle.cpp"
#include "StatsBar.cpp"
#include "Sound.cpp"


#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>



class Game{
    private:
    
    People *person;
    Shit *shit;
    Obstacle *obstacle;

    Bird *bird;

    //Vectors to store objects that will be continuously generated.
    std::vector<People*> people_objects;
    std::vector<Shit*> shit_objects;
    std::vector<Obstacle*> obstacle_objects;

    //The next group of variables are responsible for the behavior of the game in each level.

    int person_generation_speed; //the higher, the slower.
    int person_generation_counter;

    int obstacle_generation_speed; //the higher, the slower.
    int obstacle_generation_counter;

    int shitting_speed; //the higher, the slower.
    int shitting_counter;

    int recovery_speed; //the higher, the slower.
    int frame_count;

    int y_offset;
    int y_offset_factor;

    //Global flags to signal whether a function should quit, a game should quit or the game should be over.
    bool quit = false;
    bool exit_game = false;
    bool gameover = false;

    SDL_Event e;

    std::ifstream stat_file_r;
    std::ofstream stat_file_w;

    int stamina;
    int score = 0;
    int highscore = 0;

    //Variables to keep track of time in a level.
    double elapsed_seconds = 0;
    clock_t begin;
    clock_t end;

    //Used to keep a record the bird's direction.
    int direction_switch = 0;

    //Data types for the main menu function.
    SDL_Texture *menu_screens[2];
    SDL_Rect menu_clipping_rect;
    SDL_Rect menu_background_dest_rect;
    int menu_selection;

    //Data types for the game over function.
    SDL_Texture *game_over_texture;
    SDL_Rect game_over_dest_rect;
    TTF_Font *score_font;
    SDL_Color font_color;
    SDL_Surface *score_surface;
    SDL_Texture *score_texture;
    SDL_Rect score_rect;
    int score_width;
    int score_height;
    std::string score_string;


    //Sound variables to store the sounds that are to be played.
    Sound *reaction_sounds[6];
    Sound *impact_sound;
    Sound *take_off_sound;
    Sound *background_music;
    

    public:
    
    Game() {
        stamina = 100;
        score = 0;

        printf("Game Initiated.\n");

        //Read the stats file. If it has a past record, read it in. If not, move on.

        stat_file_r.open("stats.txt");
        std::string line;
        getline(stat_file_r, line);

        if (!line.empty()) {
            highscore = std::stoi(line);
            std::cout << highscore << std::endl;
        }
        
        stat_file_r.close();
        //Open file for writing.
        stat_file_w.open("stats.txt", std::ios::trunc);
    }

    void run() {
        if (!init()) {
            printf("Failed to initialize!\n");
        } else {
            if (!loadMedia()) {
                printf("Failed to load media!\n");
            } else{

                //Calling static functions from various classes to load sprites first and keep them static.
                People::loadSprites();
                Obstacle::loadSprites();
                Shit::loadSprites();

                reaction_sounds[0] = new Sound("Sounds/son of a bleep.wav", 1);
                reaction_sounds[1] = new Sound("Sounds/wtf.wav", 1);
                reaction_sounds[2] = new Sound("Sounds/oh shit male.wav", 1);
                reaction_sounds[3] = new Sound("Sounds/fuck.wav", 1);
                reaction_sounds[4] = new Sound("Sounds/oh shit female.wav", 1);
                reaction_sounds[5] = new Sound("Sounds/really female.wav", 1);
                impact_sound = new Sound("Sounds/bird hit.wav", 1);
                take_off_sound = new Sound("Sounds/bird take off.wav", 1);
                background_music = new Sound("Sounds/background.wav", 0);

                main_menu();

                if (!exit_game) {
                    level1();
                }
                if(gameover) {
                    game_over();
                } 
                else {
                    if (!exit_game) {
                        level2();
                    }
                    if(gameover) {
                        game_over();
                    } 
                    else {
                        if (!exit_game) {
                            level3();
                        }
                        game_over();
                    }
                }
            }
        }

        //If the new score is higher than the previous score, write it to the file.
        if (score > highscore) {
            stat_file_w << score;
        } else {
            stat_file_w << highscore;
        }
    }

    ~Game() {
        delete person;
        delete shit;
        delete obstacle;
        delete bird;

        people_objects.clear();
        shit_objects.clear();
        obstacle_objects.clear();
        
        SDL_DestroyTexture(menu_screens[0]);
        SDL_DestroyTexture(menu_screens[1]);
        delete menu_screens;
        
        SDL_DestroyTexture(game_over_texture);
        
        TTF_CloseFont(score_font);
        delete score_font;
        
        SDL_FreeSurface(score_surface);
        delete score_surface;

        SDL_DestroyTexture(score_texture);
        delete score_texture;
        
        for (int i=0; i<6; i++) {
            delete reaction_sounds[i];
        }
        delete reaction_sounds;

        delete impact_sound;
        delete take_off_sound;
        delete background_music;

        stat_file_w.close();
        close();
    }

    void game_over() {
            
            quit = false;
            exit_game = false;

            //Setting up the font to display the score at the end.
            score_string = "Your Score: " + std::to_string(score);
            font_color = {251, 176, 59};
            score_font = TTF_OpenFont("Fonts/Bradley_Hand_ITC_TT_Bold.ttf", 100);
            score_surface = TTF_RenderText_Solid(score_font, score_string.c_str(), font_color);
            score_texture = SDL_CreateTextureFromSurface(gRenderer, score_surface);
            TTF_SizeText(score_font, score_string.c_str(), &score_width, &score_height);

            score_rect = {400, 500, score_width, score_height};

            game_over_texture = loadTexture("Sprites/Game Over Screen.png");

            game_over_dest_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

            while( !quit ) {
                
                while( SDL_PollEvent( &e ) != 0 ) {
                    
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                        exit_game = true;
                    }
                    
                    if(e.type == SDL_KEYDOWN) {
                        if(e.key.keysym.sym == SDLK_q) {
                            quit = true;
                            exit_game = true;
                        }
                    }
                }
                
                clearScreen();
                                    
                SDL_RenderCopy(gRenderer, game_over_texture, NULL, &game_over_dest_rect);
                SDL_RenderCopy(gRenderer, score_texture, NULL, &score_rect);
                SDL_RenderPresent( gRenderer );
                

            }
    }

    void main_menu() {
        
        quit = false;

        menu_screens[0] = loadTexture("Sprites/mm1.png");
        menu_screens[1] = loadTexture("Sprites/mm2.png");

        
        menu_clipping_rect = {0, 0, 8000, 4500};

        menu_background_dest_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

        menu_selection = 0;

        while( !quit ) {
            
            while( SDL_PollEvent( &e ) != 0 ) {
                
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                
                if(e.type == SDL_KEYDOWN) {

                    if(e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN) {
                        if (menu_selection == 0) menu_selection = 1;
                        else if (menu_selection == 1) menu_selection = 0;    
                    }

                    else if(e.key.keysym.sym == SDLK_q) {
                        exit_game = true;
                        quit = true;
                    }
                    
                    else if(e.key.keysym.sym == SDLK_RETURN) {
                        if (menu_selection == 0); quit = true;//start game
                        if (menu_selection == 1) {
                            exit_game = true;
                            quit = true;
                        }
                    }
                }
            }
            
            clearScreen();
                                
            SDL_RenderCopy(gRenderer, menu_screens[menu_selection], NULL, &menu_background_dest_rect);
            SDL_RenderPresent( gRenderer );
            

        }
    }
    //All the levels have similar structure, just with different control values and backgrounds.
    void level1() {
        quit = false;
        exit_game = false;
        gameover = false;

        //Values being assigned to control variables to make each level different.
        person_generation_speed = 120; //the higher, the slower.
        person_generation_counter = 0;

        obstacle_generation_speed = 240; //the higher, the slower.
        obstacle_generation_counter = 0;

        shitting_speed; //the higher, the slower.
        shitting_counter = 0;

        recovery_speed = 60; //the higher, the slower.
        frame_count = 0;

        y_offset = 80;
        y_offset_factor = 20;

        SDL_Rect background_destination_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Texture *background_texture = loadTexture("Sprites/background1.png");

        bird = new Sparrow(gRenderer);
        shitting_speed = bird->shitting_speed;
        bird->draw();  

        background_music->play();

        StatsBar status_bar = StatsBar(gRenderer, 60, highscore);
        status_bar.draw(100, 0, 0);

        while( !quit ) {

            begin = clock(); //For timing purposes

            shitting_counter++;
            frame_count++;

            //Recover stamina
            if(frame_count >= recovery_speed && stamina < 98) {
                stamina += bird->recovery_amount;
                frame_count = 0;
            }

            while( SDL_PollEvent( &e ) != 0 ) { //Our event listener
                
                if( e.type == SDL_QUIT ) {
                    exit_game = true;
                    quit = true;
                }
                
                if(e.type == SDL_KEYDOWN) {

                    if(e.key.keysym.sym == SDLK_UP && y_offset-y_offset_factor >= 80) {
                        y_offset -= y_offset_factor;
                    }
                    else if(e.key.keysym.sym == SDLK_DOWN && y_offset <= SCREEN_HEIGHT-450-bird->mRect.h) {
                        y_offset += y_offset_factor;
                    }

                    else if(e.key.keysym.sym == SDLK_RIGHT) {
                        direction_switch = 1;
                    }

                    else if(e.key.keysym.sym == SDLK_LEFT) {
                        direction_switch = -1;
                    }
                    
                    else if(e.key.keysym.sym == SDLK_q) {
                        exit_game = true;
                        quit = true;
                    }
                    else if(e.key.keysym.sym == SDLK_SPACE) {
                        if(e.key.repeat > 0) {
                            //DROP THE SHIT BOMB
                            if(shitting_counter >= shitting_speed) {
                                shitting_counter = 0;
                                
                                stamina -= bird->damage_per_shit;
                                /* if(!DEBUG) system("cls");
                                printf("Score: %d\n", score);
                                printf("Stamina: %d\n", stamina); */
                                
                                shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                                shit_objects.push_back(shit);
                                shit->draw();
                            }
                        } else {
                            stamina -= bird->damage_per_shit;
                            /*if(!DEBUG) system("cls");
                            printf("Score: %d\n", score);
                            printf("Stamina: %d\n", stamina);*/
                            
                            shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                            shit_objects.push_back(shit);
                            shit->draw();
                        }
                    }
                }
            }
            
            clearScreen();
                                
            generatePerson();
            generateObstacle();

            manageCollisions();

            if(stamina <= 0) {
                quit = true;
                gameover = true;
                stamina = 0;
            }

            if(elapsed_seconds >= 60) {
                quit = true;
            }

            SDL_RenderCopy(gRenderer, background_texture, NULL, &background_destination_rect);
            updateObjects();
            status_bar.update(stamina, elapsed_seconds, score);

            SDL_RenderPresent( gRenderer );

            end = clock();
            elapsed_seconds += double(end - begin) / CLOCKS_PER_SEC;
        }
        background_music->stop();
        //Clear the vectors of all objects so they can be resued by the next level.
        people_objects.clear();
        obstacle_objects.clear();
        shit_objects.clear();
    }

    void level2() {
        quit = false;
        exit_game = false;
        gameover = false;

        elapsed_seconds = 0;
        stamina = 100;

        person_generation_speed = 100; //the higher, the slower.
        person_generation_counter = 0;

        obstacle_generation_speed = 180; //the higher, the slower.
        obstacle_generation_counter = 0;

        shitting_speed; //the higher, the slower.
        shitting_counter = 0;

        recovery_speed = 60; //the higher, the slower.
        frame_count = 0;

        y_offset = 80;
        y_offset_factor = 20;


        SDL_Rect background_destination_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Texture *background_texture = loadTexture("Sprites/background2.png");

        bird = new Pigeon(gRenderer);
        shitting_speed = bird->shitting_speed;
        bird->draw();  

        background_music->play();

        StatsBar status_bar = StatsBar(gRenderer, 60, highscore);
        status_bar.draw(100, 0, 0);

        while( !quit ) {

            begin = clock();

            shitting_counter++;
            frame_count++;

            //Recover stamina
            if(frame_count >= recovery_speed && stamina < 98) {
                stamina += bird->recovery_amount;
                frame_count = 0;
            }

            while( SDL_PollEvent( &e ) != 0 ) {
                
                if( e.type == SDL_QUIT ) {
                    exit_game = true;
                    quit = true;
                }
                
                if(e.type == SDL_KEYDOWN) {

                    if(e.key.keysym.sym == SDLK_UP && y_offset-y_offset_factor >= 80) {
                        y_offset -= y_offset_factor;
                    }
                    else if(e.key.keysym.sym == SDLK_DOWN && y_offset <= SCREEN_HEIGHT-450-bird->mRect.h) {
                        y_offset += y_offset_factor;
                    }

                    else if(e.key.keysym.sym == SDLK_RIGHT) {
                        direction_switch = 1;
                    }

                    else if(e.key.keysym.sym == SDLK_LEFT) {
                        direction_switch = -1;
                    }
                    
                    else if(e.key.keysym.sym == SDLK_q) {
                        exit_game = true;
                        quit = true;
                    }
                    else if(e.key.keysym.sym == SDLK_SPACE) {
                        if(e.key.repeat > 0) {
                            //DROP THE SHIT BOMB
                            if(shitting_counter >= shitting_speed) {
                                shitting_counter = 0;
                                
                                stamina -= bird->damage_per_shit;
                                /* if(!DEBUG) system("cls");
                                printf("Score: %d\n", score);
                                printf("Stamina: %d\n", stamina); */
                                
                                shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                                shit_objects.push_back(shit);
                                shit->draw();
                            }
                        } else {
                            stamina -= bird->damage_per_shit;
                            /*if(!DEBUG) system("cls");
                            printf("Score: %d\n", score);
                            printf("Stamina: %d\n", stamina);*/
                            
                            shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                            shit_objects.push_back(shit);
                            shit->draw();
                        }
                    }
                }
            }
            
            clearScreen();
                                
            generatePerson();
            generateObstacle();

            manageCollisions();

            if(stamina <= 0) {
                quit = true;
                gameover = true;
                stamina = 0;
            }

            if(elapsed_seconds >= 60) {
                quit = true;
            }

            SDL_RenderCopy(gRenderer, background_texture, NULL, &background_destination_rect);
            updateObjects();
            status_bar.update(stamina, elapsed_seconds, score);

            SDL_RenderPresent( gRenderer );

            end = clock();
            elapsed_seconds += double(end - begin) / CLOCKS_PER_SEC;
        }
        background_music->stop();
        
        people_objects.clear();
        obstacle_objects.clear();
        shit_objects.clear();
    }

    void level3() {
        quit = false;
        exit_game = false;
        gameover = false;

        elapsed_seconds = 0;
        stamina = 100;

        person_generation_speed = 120; //the higher, the slower.
        person_generation_counter = 0;

        obstacle_generation_speed = 120; //the higher, the slower.
        obstacle_generation_counter = 0;

        shitting_speed; //the higher, the slower.
        shitting_counter = 0;

        recovery_speed = 60; //the higher, the slower.
        frame_count = 0;

        y_offset = 80;
        y_offset_factor = 20;


        SDL_Rect background_destination_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Texture *background_texture = loadTexture("Sprites/background3.png");

        bird = new Pigeon(gRenderer);
        shitting_speed = bird->shitting_speed;
        bird->draw();  

        background_music->play();

        StatsBar status_bar = StatsBar(gRenderer, 60, highscore);
        status_bar.draw(100, 0, 0);

        while( !quit ) {

            begin = clock();

            shitting_counter++;
            frame_count++;

            //Recover stamina
            if(frame_count >= recovery_speed && stamina < 98) {
                stamina += bird->recovery_amount;
                frame_count = 0;
            }

            while( SDL_PollEvent( &e ) != 0 ) {
                
                if( e.type == SDL_QUIT ) {
                    exit_game = true;
                    quit = true;
                }
                
                if(e.type == SDL_KEYDOWN) {

                    if(e.key.keysym.sym == SDLK_UP && y_offset-y_offset_factor >= 80) {
                        y_offset -= y_offset_factor;
                    }
                    else if(e.key.keysym.sym == SDLK_DOWN && y_offset <= SCREEN_HEIGHT-450-bird->mRect.h) {
                        y_offset += y_offset_factor;
                    }

                    else if(e.key.keysym.sym == SDLK_RIGHT) {
                        direction_switch = 1;
                    }

                    else if(e.key.keysym.sym == SDLK_LEFT) {
                        direction_switch = -1;
                    }
                    
                    else if(e.key.keysym.sym == SDLK_q) {
                        exit_game = true;
                        quit = true;
                    }
                    else if(e.key.keysym.sym == SDLK_SPACE) {
                        if(e.key.repeat > 0) {
                            //DROP THE SHIT BOMB
                            if(shitting_counter >= shitting_speed) {
                                shitting_counter = 0;
                                
                                stamina -= bird->damage_per_shit;
                                /* if(!DEBUG) system("cls");
                                printf("Score: %d\n", score);
                                printf("Stamina: %d\n", stamina); */
                                
                                shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                                shit_objects.push_back(shit);
                                shit->draw();
                            }
                        } else {
                            stamina -= bird->damage_per_shit;
                            /*if(!DEBUG) system("cls");
                            printf("Score: %d\n", score);
                            printf("Stamina: %d\n", stamina);*/
                            
                            shit = new Shit(gRenderer, bird->mRect.x + (bird->mRect.w/2), bird->mRect.y + bird->mRect.h);
                            shit_objects.push_back(shit);
                            shit->draw();
                        }
                    }
                }
            }
            
            clearScreen();
                                
            generatePerson();
            generateObstacle();

            manageCollisions();

            if(stamina <= 0) {
                quit = true;
                gameover = true;
                stamina = 0;
            }

            if(elapsed_seconds >= 60) {
                quit = true;
            }

            SDL_RenderCopy(gRenderer, background_texture, NULL, &background_destination_rect);
            updateObjects();
            status_bar.update(stamina, elapsed_seconds, score);

            SDL_RenderPresent( gRenderer );

            end = clock();
            elapsed_seconds += double(end - begin) / CLOCKS_PER_SEC;
        }
        background_music->stop();
        
        people_objects.clear();
        obstacle_objects.clear();
        shit_objects.clear();
    }

    int checkCollision(SDL_Rect *rectA, SDL_Rect *rectB) { //A wrapper function for SDL's own Rectangle Intersection function.
        SDL_bool result = SDL_HasIntersection(rectA, rectB);

	    if(result == SDL_TRUE) {
            return 1;
        } else {
            return 0;
        }
    }

    void generatePerson() { //Our class factory that generates people objects at a rate determined by control variables.
        person_generation_counter++;

        if(person_generation_counter == person_generation_speed) {
            person_generation_counter = 0;

            //Create a new person
            person = new People(gRenderer);
            people_objects.push_back(person);
            person->draw();

        }
    }

    void generateObstacle() { //Class factory that generates obstacle objects at a rate determined by control variables.
        obstacle_generation_counter++;

        if(obstacle_generation_counter == obstacle_generation_speed) {
            obstacle_generation_counter = 0;

            //Create a new person
            obstacle = new Obstacle(gRenderer);
            obstacle_objects.push_back(obstacle);
            obstacle->draw();

        }
    }

    void manageCollisions() { //Collision Detection engine responsible for determining whether any of the objects have collided or not.
        int collision = 0;

        /*  Check Collisions: 
            Iterate through all objects and see if there's a collision.
            If so, delete the objects that collided from everywhere. 
        */

        //Check collision between shits and people.
        for (auto it = shit_objects.begin(); it != shit_objects.end();) {
            for(auto i = people_objects.begin(); i !=people_objects.end();) {
                
                collision = checkCollision(&(*it)->mRect, &(*i)->mRect);

                if (collision == 1) {
                    //collision found
                    if ((*i)->gender == 0) {
                        //male
                        reaction_sounds[rand() % 4]->play();
                    }
                    else if ((*i)->gender == 1) {
                        //female
                        reaction_sounds[4 + (rand() % 2)]->play();
                    }
                    
                    score += 5;

                    delete (*it);
                    shit_objects.erase(it);
                    delete (*i);
                    people_objects.erase(i);
                    break;

                    //After a collision, check if any of the vectors are empty. If so, stop the check.
                    if (people_objects.empty() || shit_objects.empty()) break;

                } else {
                    i++;
                }
            }
            if (people_objects.empty() || shit_objects.empty()) break;
            if(collision == 0) {
                it++; //there hasn't been a deletion, increment. 
            } else {
                break;
            }
        }


        //Check collision between shits and obstacles.
        for (auto it = shit_objects.begin(); it != shit_objects.end();) {
            for(auto i = obstacle_objects.begin(); i !=obstacle_objects.end();) {
                    
                    collision = checkCollision(&(*it)->mRect, &(*i)->mRect);

                    if (collision == 1) {
                        //collision found
                        /* if(DEBUG) printf("Collision! Shit Deleted, Obstacle Deleted.\n"); */
                        delete (*it);
                        shit_objects.erase(it);
                        delete (*i);
                        obstacle_objects.erase(i);

                        break;

                        //After a collision, check if any of the vectors are empty. If so, stop the check.
                        if (obstacle_objects.empty() || shit_objects.empty()) break;

                    } else {
                        i++;
                    }
            }
            if (obstacle_objects.empty() || shit_objects.empty()) break;
            if(collision == 0) {
                it++; //there hasn't been a deletion, increment.
            } else{
                break;
            }
        }


        //Check collision between bird and obstacles.
        for(auto i = obstacle_objects.begin(); i !=obstacle_objects.end();) {
                
                collision = checkCollision(&bird->mRect, &(*i)->mRect);

                if (collision == 1) {
                    //collision found
                    stamina -= bird->damage_per_hit;
                    impact_sound->play();
                    
                    delete (*i);
                    obstacle_objects.erase(i);

                    //After a collision, check if any of the vectors are empty. If so, stop the check.
                    if (obstacle_objects.empty()) break;
                    break;

                } else {
                    i++;
                }
        }
    }

    void updateObjects() { //Responsible for updating the position of all the objects stored in our vectors and determining their position in their life cycle.
        
        bird->update(y_offset, direction_switch);
        direction_switch = 0;

        //Update position of all shit objects
        for (auto it = shit_objects.begin(); it != shit_objects.end();) {
            if ((*it)->update() == 1) {
                delete (*it);
                shit_objects.erase(it);
                /* if(DEBUG) std::cout << "Shit deleted." << std::endl; */
            }
            else {
                it++;
            }
        }

        //Update position of all people objects
        for (auto it = people_objects.begin(); it != people_objects.end();) {

            if ((*it)->update() == 1) {
                delete (*it);
                people_objects.erase(it);
                /* if(DEBUG) std::cout << "Person deleted." << std::endl; */
            }
            else {
                it++;
            }
        }

        //Update position of all obstacle objects
        for (auto it = obstacle_objects.begin(); it != obstacle_objects.end();) {

            if ((*it)->update() == 1) {
                delete (*it);
                obstacle_objects.erase(it);
                /* if(DEBUG) std::cout << "Obstacle deleted." << std::endl; */
            }
            else {
                it++;
            }
        }
    }

    void clearScreen() { 
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
    }
};