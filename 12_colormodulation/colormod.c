#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
struct LTexture gModulatedTexture;

typedef char bool;
#define true 1
#define false 0
typedef struct LTexture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
}LTexture;

bool LT_loadFromFile(struct LTexture *texture, const char* path);
void LT_free(struct LTexture *texture);
void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip);
void LT_setColor(struct LTexture *texture, uint8_t red, uint8_t green, uint8_t blue);
bool MYinit();
bool loadMedia();
void MYclose();

bool LT_loadFromFile(struct LTexture *texture, const char* path) {
	LT_free(texture);
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_IMG_Error: %s\n", path, IMG_GetError());
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to load image from %s! SDL_Error: %s\n", path, SDL_GetError());
		}
		else {
			texture->mWidth = loadedSurface->w;
			texture->mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

void LT_free(struct LTexture *texture) {
	if(texture->mTexture != NULL) {
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
		texture->mWidth = 0;
		texture->mHeight = 0;
	}
}

void LT_setColor(struct LTexture *texture, uint8_t red, uint8_t green, uint8_t blue) {
	SDL_SetTextureColorMod(texture->mTexture, red, green, blue);
}

void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip) {
	SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy(gRenderer, texture->mTexture, clip, &renderQuad);
}

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
		gWindow = SDL_CreateWindow("c0l0r m0d!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! %s\n", SDL_GetError());
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
	if(!LT_loadFromFile(&gModulatedTexture, "colors.png")) {
		printf("Failed to load colors texture!\n");
		success = false;
	}
	return success;
}

void MYclose() {
	LT_free(&gModulatedTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

int(main(int argc, char* args[])) {
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
			uint8_t r = 255;
			uint8_t g = 255;
			uint8_t b = 255;
			while(!quit) {
				while(SDL_PollEvent(&event) != 0) {
					if(event.type == SDL_QUIT) {
						quit = true;
					}
					else if(event.type == SDL_KEYDOWN) {
						switch(event.key.keysym.sym) {
							case SDLK_q:
								r += 32;
								break;
							case SDLK_w:
								g += 32;
								break;
							case SDLK_e:
								b += 32;
								break;
							case SDLK_a:
								r -= 32;
								break;
							case SDLK_s:
								g -= 32;
								break;
							case SDLK_d:
								b -= 32;
								break;
						}
					}
				}
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				LT_setColor(&gModulatedTexture, r, g, b);
				LT_render(&gModulatedTexture, 0, 0, NULL);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
