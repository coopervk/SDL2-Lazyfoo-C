#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef char bool;
#define true 1
#define false 0
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gPNGSurface = NULL;

bool MYinit();
bool loadMedia();
void MYclose();
SDL_Surface* loadSurface(const char* path);

bool MYinit() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("PNG time!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_Image could not be initialized! SDL_IMG_Error: %s\n", IMG_GetError());
				success = false;
			}
			else {
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gPNGSurface = loadSurface("loaded.png");
	if(gPNGSurface == NULL) {
		printf("Failed to load PNG image!\n");
		success = false;
	}
	return success;
}

void MYclose() {
	SDL_FreeSurface(gPNGSurface);
	gPNGSurface = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(const char* path) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_IMG_Error: %s\n", path, IMG_GetError());
	}
	else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, (*gScreenSurface).format, 0);
		if(optimizedSurface == NULL) {
			printf("Unable to optimize image %s! SDL_Error: %s\n", path, SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL; // Tutorial forgot this!
	}
	return optimizedSurface;
}

int main(int argc, char* args[]) {
	if(!MYinit()) {
		printf("Failed to initialize!\n");
	}
	else {
		if(!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			bool quit = false;
			SDL_Event event;
			while(!quit) {
				while(SDL_PollEvent(&event) != 0) {
					if(event.type == SDL_QUIT) {
						quit = true;
					}
				}
				SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	MYclose();
	return 0;
}
