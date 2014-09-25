//Headers
#include <iostream>
#include <SDL.h> //SDL functionality

//Variables

//Pointer to our window
SDL_Window * window;

//Window creation properties
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

//Fullscreen?
bool full = false;

//Functions

//Window
void InitWindow(int width, int height, bool fullscreen)
{
	window = SDL_CreateWindow(
		"Lab 1",				//Window title
		SDL_WINDOWPOS_CENTERED, //x position of the window - centered
		SDL_WINDOWPOS_CENTERED, //y position of the window - againt centered
		width,					//Window width, in pixels
		height,					//Window height, in pixels
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);     //Flags ?? 
}



//Clean up
void CleanUp()
{
	SDL_DestroyWindow(window);
	SDL_Quit;
}

//Main method - entry point
int main(int argc, char * arg[])
{
	//Initialise window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, full);

	//Initiate SDL
	if(SDL_Init (SDL_INIT_EVERYTHING) != 0)
	{
		std::cout <<"ERROR SDL_Init" <<SDL_GetError() << std::endl;

		//If it fucks up ...
		return -1;
	}

	CleanUp();
	return 0;
}

