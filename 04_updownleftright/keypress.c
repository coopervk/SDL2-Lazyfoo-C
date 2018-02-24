#include <SDL2/SDL.h>
#include <stdio.h>

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* gCurrentSurface = NULL;

typedef char bool;
#define true 1
#define false 0

bool MYinit();
bool loadMedia();
void MYclose();
SDL_Surface* MYloadSurface(const char* path);

bool MYinit() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO < 0)) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("SDL key presses", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("SDL could not create a windo! SDL_Error %s\n", SDL_GetError());
			success = false;
		}
		else
			gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = MYloadSurface("press.bmp");
	if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
		printf("SDL could not load DEFAULT press image!\n");
		success = false;
	}
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = MYloadSurface("up.bmp");
	if(gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		printf("SDL could not load UP press image!\n");
		success = false;
	}
	
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = MYloadSurface("down.bmp");
	if(gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		printf("SDL could not load DOWN press image!\n");
		success = false;
	}
	
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = MYloadSurface("left.bmp");
	if(gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		printf("SDL could not load LEFT press image!\n");
		success = false;
	}
	
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = MYloadSurface("right.bmp");
	if(gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		printf("SDL could not load RIGHT press image!\n");
		success = false;
	}
	return success;
}

void MYclose() {
	int i = 0;
	for(i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		SDL_FreeSurface(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

SDL_Surface* MYloadSurface(const char* path) {
	SDL_Surface* loadedSurface = SDL_LoadBMP(path);
	if(loadedSurface == NULL)
		printf("Unable to load %s! SDL_Error: %s\n", path, SDL_GetError());
	return loadedSurface;
}

int main(int argc, char* args[]) {
	if(!MYinit())
		printf("Initialization failed!\n");
	else {
		if(!loadMedia())
			printf("Media loading failed!\n");
		else {
			bool quit = false;
			SDL_Event event;
			gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
			while(!quit) {
				while(SDL_PollEvent(&event) != 0) {
					if(event.type == SDL_QUIT)
						quit = true;
					else if(event.type == SDL_KEYDOWN) {
						switch(event.key.keysym.sym) {
							case SDLK_UP:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
								break;

							case SDLK_DOWN:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
								break;

							case SDLK_LEFT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
								break;

							case SDLK_RIGHT:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
								break;

							default:
								gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
								break;
						}
					}
				}
				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	MYclose();
	return 0;
}
