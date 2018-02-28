#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
TTF_Font *gFont = NULL;
struct LTexture gTextTexture;

bool LT_loadFromFile(struct LTexture *texture, const char* path);
bool LT_loadFromRenderedText(struct LTexture *texture, const char* textureText, SDL_Color textColor);
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
			printf("Unable to create texture from %s! SDL_Error: %s\n", path, SDL_GetError());
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

bool LT_loadFromRenderedText(struct LTexture *texture, const char* textureText, SDL_Color textColor) {
	LT_free(texture);
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
	if(textSurface == NULL) {
		printf("Unable to render text surface! SDL_TTF_Error: %s\n", TTF_GetError());
	}
	else {
		texture->mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(texture->mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			texture->mWidth = textSurface->w;
			texture->mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	return texture->mTexture != NULL;
}

void LT_free(struct LTexture *texture) {
	if(texture->mTexture != NULL) {
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
		texture->mWidth = 0;
		texture->mHeight =0;
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
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning! Linear texture filtering not enabled!");
		}
		gWindow = SDL_CreateWindow("type it up", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
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
					printf("SDL_IMG could not initialized! SDL_IMG_Error: %s\n", IMG_GetError());
					success = false;
				}
				if(TTF_Init() == -1) {
					printf("SDL_TTF could not be initialized! SDL_TTF_Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	gFont = TTF_OpenFont("lazy.ttf", 28);
	if(gFont == NULL) {
		printf("Failed to load lazy font! SDL_TTF_Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		SDL_Color textColor = {0, 0, 0};
		if(!LT_loadFromRenderedText(&gTextTexture, "The quick brown fox jumps over the lazy dog", textColor)) {
			printf("Failed to render text texture!");
			success = false;
		}
	}
	return success;
}

void MYclose() {
	LT_free(&gTextTexture);
	TTF_CloseFont(gFont);
	gFont = NULL;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	TTF_Quit();
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
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);
					LT_render(&gTextTexture, (SCREEN_WIDTH - gTextTexture.mWidth) / 2, (SCREEN_HEIGHT - gTextTexture.mHeight)/2, NULL, 0.0, NULL, SDL_FLIP_NONE);
					SDL_RenderPresent(gRenderer);
				}
			}
		}
	}
	MYclose();
	return 0;
}
