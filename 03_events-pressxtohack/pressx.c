#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gXOut = NULL;

typedef char bool;
#define true 1
#define false 0

bool MYinit();
bool loadMedia();
void MYclose();

bool MYinit() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("SDL take input", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("SDL could not initialize window! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gXOut = SDL_LoadBMP("x.bmp");
	if(gXOut == NULL) {
		printf("Unable to load image %s! SDL_Error: %s\n", "x.bmp", SDL_GetError());
		success = false;
	}
	return success;
}

void MYclose() {
	SDL_FreeSurface(gXOut);
	gXOut = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if(!MYinit())
		printf("Failed to initialize!\n");
	else {
		if(!loadMedia())
			printf("Failed to load media!\n");
		else {
			bool quit = false;
			SDL_Event event;
			while(!quit) {
				while(SDL_PollEvent(&event) != 0)
					if(event.type == SDL_QUIT)
						quit = true;
				SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	MYclose();
	return 0;
}

