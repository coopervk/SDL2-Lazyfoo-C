/* # # # # # Includes # # # # # */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* # # # # # Typedefs # # # # # */
typedef char bool;
#define true 1
#define false 0

typedef struct LTexture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
}LTexture;

/* # # # # # Variable instantiation # # # # # */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
struct LTexture gFooTexture = {.mTexture = NULL, .mWidth = 0, .mHeight = 0};
struct LTexture gBackgroundTexture = {.mTexture = NULL, .mWidth = 0, .mHeight = 0};

/* # # # # # Function prototypes # # # # # */
bool MYinit();
bool loadMedia();
void MYclose();
void LT_free(struct LTexture *texture);
bool LT_loadFromFile(struct LTexture *texture, const char* path);
void LT_render(struct LTexture *texture, int x, int y);
int LT_getWidth(struct LTexture *texture);
int LT_getHeight(struct LTexture *texture);

/* # # # # # LT functions # # # # # */
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

void LT_render(struct LTexture *texture, int x, int y) {
//	printf("%d, %d", x, y);
	SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
	SDL_RenderCopy(gRenderer, texture->mTexture, NULL, &renderQuad);
}

int LT_getWidth(struct LTexture *texture)  { return texture->mWidth; }

int LT_getHeight(struct LTexture *texture) { return texture->mHeight; }

/* # # # # # Other functions # # # # # */
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
		gWindow = SDL_CreateWindow("Key lime cove", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL) {
				printf("Renderer could not be initialized! SDL_Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_Image could not be initialized! SDL_IMG_Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	if(!LT_loadFromFile(&gFooTexture, "foo.png")) {
		printf("Failed to load foreground image!\n");
		success = false;
	}
	if(!LT_loadFromFile(&gBackgroundTexture, "background.png")) {
		printf("Failed to load background image!\n");
		success = false;
	}
	if(gFooTexture.mTexture == NULL) {
		printf("DAVE0 WAS RIGHT");
	}
	return success;
}

void MYclose() {
	LT_free(&gFooTexture);
	LT_free(&gBackgroundTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;
	IMG_Quit();
	SDL_Quit();
}

/* # # # # # Main # # # # # */
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
				SDL_SetRenderDrawColor(gRenderer, 0x50, 0x50, 0x50, 0xFF);
				SDL_RenderClear(gRenderer);
				LT_render(&gBackgroundTexture, 0, 0);
				LT_render(&gFooTexture, 240, 190);
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
