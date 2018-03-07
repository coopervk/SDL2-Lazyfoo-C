#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef char bool;
#define true 1
#define false 0
typedef struct LTexture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
}LTexture;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gPressTexture;
LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;

bool LT_loadFromFile(struct LTexture *texture, const char* path);
void LT_free(struct LTexture *texture);
void LT_setColor(struct LTexture *texture, uint8_t red, uint8_t green, uint8_t blue);
void LT_setBlendMode(struct LTexture *texture, SDL_BlendMode blending);
void LT_setAlpha(struct LTexture *texture, uint8_t alpha);
void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
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
			printf("Unable to create texture from %s SDL_Error: %s\n", path, SDL_GetError());
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

void LT_setBlendMode(struct LTexture *texture, SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texture->mTexture, blending);
}

void LT_setAlpha(struct LTexture *texture, uint8_t alpha) {
	SDL_SetTextureAlphaMod(texture->mTexture, alpha);
}

void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
	if(clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(gRenderer, texture->mTexture, clip, &renderQuad, angle, center, flip);
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
		gWindow = SDL_CreateWindow("keyboi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be initialized! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL) {
				printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_IMG could not be initialized! SDL_IMG_Error: %s\n", IMG_GetError());
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	if(!LT_loadFromFile(&gPressTexture, "press.png")) {
		printf("Failed to load press texture!\n");
		success = false;
	}
	if(!LT_loadFromFile(&gUpTexture, "up.png")) {
		printf("Failed to load up texture!\n");
		success = false;
	}
	if(!LT_loadFromFile(&gDownTexture, "down.png")) {
		printf("Failed to load down texture!\n");
		success = false;
	}
	if(!LT_loadFromFile(&gLeftTexture, "left.png")) {
		printf("Failed to load left texture!\n");
		success = false;
	}
	if(!LT_loadFromFile(&gRightTexture, "right.png")) {
		printf("Failed to load right texture!\n");
		success = false;
	}
	return success;
}

void MYclose() {
	LT_free(&gPressTexture);
	LT_free(&gUpTexture);
	LT_free(&gDownTexture);
	LT_free(&gLeftTexture);
	LT_free(&gRightTexture);
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
			LTexture* currentTexture = (LTexture*)malloc(sizeof(LTexture));
			while(!quit) {
				while(SDL_PollEvent(&event) != 0) {
					if(event.type == SDL_QUIT) {
						quit = true;
					}
				}
				const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
				if(currentKeyStates[SDL_SCANCODE_UP]) {
					currentTexture = &gUpTexture;
				}
				else if(currentKeyStates[SDL_SCANCODE_DOWN]) {
					currentTexture = &gDownTexture;
				}
				else if(currentKeyStates[SDL_SCANCODE_LEFT]) {
					currentTexture = &gLeftTexture;
				}
				else if(currentKeyStates[SDL_SCANCODE_RIGHT]) {
					currentTexture = &gRightTexture;
				}
				else {
					currentTexture = &gPressTexture;
				}
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				LT_render(currentTexture, 0, 0, NULL, 0.0, NULL, SDL_FLIP_NONE);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
