#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Rect gSpriteClips[4];
struct LTexture gSpriteSheetTexture;

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
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB((*loadedSurface).format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
		}
		else {
			texture->mWidth = (*loadedSurface).w;
			texture->mHeight = (*loadedSurface).h;
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
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("key, clip, and sprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
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
	if(!LT_loadFromFile(&gSpriteSheetTexture, "dots.png")) {
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else {
		// Top left
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;
		// Top right
		gSpriteClips[1].x = 100;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;
		// Bottom left
		gSpriteClips[2].x = 0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;
		// Bottom right
		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}
	return success;
}

void MYclose() {
	LT_free(&gSpriteSheetTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
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
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				LT_render(&gSpriteSheetTexture, 0, 0, &gSpriteClips[0]);
				LT_render(&gSpriteSheetTexture, SCREEN_WIDTH-gSpriteClips[1].w, 0, &gSpriteClips[1]);
				LT_render(&gSpriteSheetTexture, 0, SCREEN_HEIGHT-gSpriteClips[2].h, &gSpriteClips[2]);
				LT_render(&gSpriteSheetTexture, SCREEN_WIDTH-gSpriteClips[3].w, SCREEN_HEIGHT-gSpriteClips[3].h, &gSpriteClips[3]);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
