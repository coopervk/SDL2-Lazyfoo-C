#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef char bool;
#define true 1
#define false 0
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

bool MYinit();
bool loadMedia();
void MYclose();
SDL_Texture* loadTexture(const char* path);

bool MYinit() {
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("G e o m e t r y", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be initialized! SDL_Error %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_IMG could not be initialized! SDL_IMG_Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	return success;
}

void MYclose() {
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(const char* path) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_IMG_Error: %s\n", path, IMG_GetError());
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

int main(int argc, char* args[]) {
	if(!MYinit()) {
		printf("Failed to intialized!\n");
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
				// Clear canvas
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				// Fill a quadrant with red
				SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0x00);
				SDL_RenderFillRect(gRenderer, &fillRect);
				// Fill a quadrant with green
				SDL_Rect outlineRect = {SCREEN_WIDTH/6, SCREEN_HEIGHT/6, SCREEN_WIDTH*2/3, SCREEN_HEIGHT*2/3};
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderDrawRect(gRenderer, &outlineRect);
				// Horizontal blue line
				SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);
				// Vertical dotted yellow line
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
				for(int i = 0; i < SCREEN_HEIGHT; i+= 4) {
					SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH/2, i);
				}
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}

