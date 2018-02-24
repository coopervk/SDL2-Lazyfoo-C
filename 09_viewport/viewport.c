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
SDL_Texture* gTexture = NULL;

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
			printf("Warning! Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("Viewp0rt", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
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
					printf("SDL_IMG could not initialize! SDL_IMG_Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gTexture = loadTexture("viewport.png");
	if(gTexture == NULL) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	return success;
}

void MYclose() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
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
		printf("Unable to load image %s! SDL_IMG_Error: %s\n", path, SDL_GetError());
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
		printf("Failed to initialize!\n");
	}
	else {
		if(!loadMedia()) {
			printf("Failed to load media!");
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
				// Clear
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				// Top left
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0; 
				topLeftViewport.y = 0;
				topLeftViewport.w = SCREEN_WIDTH/2;
				topLeftViewport.h = SCREEN_HEIGHT/2;
				SDL_RenderSetViewport(gRenderer, &topLeftViewport);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				// Top right
				SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH/2;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH/2;
				topRightViewport.h = SCREEN_HEIGHT/2;
				SDL_RenderSetViewport(gRenderer, &topRightViewport);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				// Bottom
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT/2;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT/2;
				SDL_RenderSetViewport(gRenderer, &bottomViewport);
				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
				// Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
