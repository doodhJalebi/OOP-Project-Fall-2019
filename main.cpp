#include <string>
#include <iostream>
#include <stdio.h>
#include <cmath>

#include "Game.cpp"


//Just compile the main.cpp and all other classes will be linked automatically.

int main( int argc, char* args[] )
{
	Game game;
	game.run();
	
	return 0;
}