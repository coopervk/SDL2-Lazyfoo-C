#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

// Create bool. You can use #include <stdbool.h> if you're a C99 d00d
typedef char bool;
#define true 1
#define false 0

bool MYinit(); // No namespaces in C! Watch for conflict!
bool loadMedia();
void MYclose(); // No namespaces in C! Watch for conflict!

bool MYinit() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not MYinitialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
			gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gHelloWorld = SDL_LoadBMP("hello_world.bmp");
	if(gHelloWorld == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError());
		success = false;
	}
	return success;
}

void MYclose() {
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if(!MYinit())
		printf("Failed to MYinitialize SDL!\n");
	else {
		if(!loadMedia())
			printf( "Failed to load media!\n" );
		else {
			SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
			SDL_UpdateWindowSurface(gWindow);
			SDL_Delay(6000);
		}
	}
	MYclose();
	return 0;
}

