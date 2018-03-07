#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef char bool;
#define true 1
#define false 0

enum LButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

typedef struct LTexture {
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
}LTexture;

typedef struct LButton {
	SDL_Point mPosition;
	enum LButtonSprite mCurrentSprite;
}LButton;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
#define TOTAL_BUTTONS 4

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
struct LTexture gButtonSpriteSheetTexture;
struct LButton gButtons[TOTAL_BUTTONS];

bool LT_loadFromFile(struct LTexture *texture, const char* path);
#ifdef _SDL_TTF_H
bool LT_loadFromRenderedText(struct LTexture *texture, const char* textureText, SDL_Color textColor);
#endif
void LT_free(struct LTexture *texture);
void LT_setColor(struct LTexture *texture, uint8_t red, uint8_t green, uint8_t blue);
void LT_setBlendMode(struct LTexture *texture, SDL_BlendMode blending);
void LT_setAlpha(struct LTexture *texture, uint8_t alpha);
void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip);
void LB_setPosition(struct LButton *mouse, int x, int y);
void LB_handleEvent(struct LButton *mouse, SDL_Event* event);
void LB_render(struct LButton *mouse);
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

#ifdef _SDL_TTF_H
bool LT_loadFromRenderedText(struct LTexture *texture, const char* textureText, SDL_Color textColor) {
	LT_free(texture);
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
	if(textSurface == NULL) {
		printf("Unable to render text surface! SDL_TTF_Error: %s\n", SDL_GetError());
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
#endif

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

void LB_setPosition(struct LButton *mouse, int x, int y) {
	mouse->mPosition.x = x;
	mouse->mPosition.y = y;
}

void LB_handleEvent(struct LButton *mouse, SDL_Event* event) {
	if(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		// Far left
		if(x < mouse->mPosition.x) {
			inside = false;
		}
		// Far right
		else if(x > mouse->mPosition.x + BUTTON_WIDTH) {
			inside = false;
		}
		// Above
		else if(y < mouse->mPosition.y) {
			inside = false;
		}
		// Below
		else if(y > mouse->mPosition.y + BUTTON_HEIGHT) {
			inside = false;
		}
		if(!inside) {
			mouse->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		// Inside
		else {
			switch(event->type) {
				case SDL_MOUSEMOTION:
					mouse->mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouse->mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;
				case SDL_MOUSEBUTTONUP:
					mouse->mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
			}
		}
	}
}

void LB_render(struct LButton *mouse) {
	LT_render(&gButtonSpriteSheetTexture, mouse->mPosition.x, mouse->mPosition.y, &gSpriteClips[mouse->mCurrentSprite], 0.0, NULL, SDL_FLIP_NONE);
}
void LT_render(struct LTexture *texture, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip);

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
		gWindow = SDL_CreateWindow("maus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
					success = false;
				}
			}
		}
	}
	return success;
}

bool loadMedia() {
	bool success = true;
	if(!LT_loadFromFile(&gButtonSpriteSheetTexture, "button.png")) {
		printf("Failed to load button sprite texture!\n");
		success = false;
	}
	else {
		for(int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i*200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}
		LB_setPosition(&gButtons[0], 0, 0);
		LB_setPosition(&gButtons[1], SCREEN_WIDTH - BUTTON_WIDTH, 0);
		LB_setPosition(&gButtons[2], 0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		LB_setPosition(&gButtons[3], SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
	}
	return success;
}

void MYclose() {
	LT_free(&gButtonSpriteSheetTexture);
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
#ifdef _SDL_TTF_H
	TTF_Quit();
#endif
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if(!MYinit()) {
		printf("Failed to initialize!");
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
					for(int i = 0; i < TOTAL_BUTTONS; ++i) {
						LB_handleEvent(&gButtons[i], &event);
					}
				}
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				for(int i = 0; i < TOTAL_BUTTONS; ++i) {
					LB_render(&gButtons[i]);
				}
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	MYclose();
	return 0;
}
