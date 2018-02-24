/* This is based upon the Lazy Foo' Productions code */

#include <SDL2/SDL.h>
#include <stdio.h>

// I like to think this was inspired by TempleOS
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
	else {
		window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(window == NULL)
			printf("SDL could not create a window! SDL_Error: %s\n", SDL_GetError());
		else {
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(6000);
		}
	}
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
