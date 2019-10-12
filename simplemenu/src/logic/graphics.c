#include "../headers/graphics.h"

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mouse.h>

#include "../headers/definitions.h"
#include "../headers/globals.h"

SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
TTF_Font *BIGFont = NULL;
TTF_Font *loadingFont = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

int calculateProportionalSizeOrDistance(int number) {
	return (SCREEN_HEIGHT*number)/240;
}

int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align, SDL_Color backgroundColor) {
	SDL_Surface *msg;
	msg = TTF_RenderText_Shaded(font, buf, txtColor, backgroundColor);
	if (align & HAlignCenter) {
		x -= msg->w / 2;
	} else if (align & HAlignRight) {
		x -= msg->w;
	}
	if (align & VAlignMiddle) {
		y -= msg->h / 2;
	} else if (align & VAlignTop) {
		y -= msg->h;
	}
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = msg->w;
	rect.h = msg->h;
	SDL_BlitSurface(msg, NULL, screen, &rect);
	SDL_FreeSurface(msg);
	return msg->w;
}

int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align) {
	SDL_Surface *msg;
	msg = TTF_RenderText_Blended(font, buf, txtColor);
	if (align & HAlignCenter) {
		x -= msg->w / 2;
	} else if (align & HAlignRight) {
		x -= msg->w;
	}
	if (align & VAlignMiddle) {
		y -= msg->h / 2;
	} else if (align & VAlignTop) {
		y -= msg->h;
	}
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = msg->w;
	rect.h = msg->h;
	SDL_BlitSurface(msg, NULL, screen, &rect);
	SDL_FreeSurface(msg);
	return msg->w;
}

void drawShadedGameNameOnScreen(char *buf, int position) {
	drawShadedTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodySelectedTextForegroundColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
}

void drawNonShadedGameNameOnScreen(char *buf, int position) {
	drawTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodyTextForegroundColor, VAlignBottom | HAlignCenter);
}

void drawPictureTextOnScreen(char *buf) {
	SDL_Color white;
	white.r=255;
	white.g=255;
	white.b=255;
	drawTextOnScreen(font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(239), buf, white, VAlignTop | HAlignCenter);
}

void drawImgFallbackTextOnScreen(char *fallBackText) {
	SDL_Color white;
	white.r=255;
	white.g=255;
	white.b=255;
	drawTextOnScreen(font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(120), fallBackText, white, VAlignTop | HAlignCenter);
}

void drawTextOnFooter(const char text[64]) {
	drawTextOnScreen(footerFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), text, menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignMiddle | HAlignCenter);
}

void drawTextOnHeader(char *text) {
	drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(24), text, menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);
}

void drawCurrentLetter(char *letter, SDL_Color textColor) {
	drawTextOnScreen(BIGFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), letter, textColor, VAlignMiddle | HAlignCenter);
}

void drawLoadingSign(char *text, SDL_Color textColor) {
	drawTextOnScreen(loadingFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), text, textColor, VAlignMiddle | HAlignCenter);
}

SDL_Rect drawRectangleOnScreen(int width, int height, int x, int y, int rgbColor[]) {
	SDL_Rect rectangle;
	rectangle.w = width;
	rectangle.h = height;
	rectangle.x = x;
	rectangle.y = y;
	SDL_FillRect(screen, &rectangle, SDL_MapRGB(screen->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	return(rectangle);
}

SDL_Surface *loadImage (char *fileName) {
	SDL_Surface *img = NULL;
	SDL_Surface *_img = IMG_Load(fileName);
	if (_img!=NULL) {
		img = SDL_DisplayFormatAlpha(_img);
		SDL_FreeSurface(_img);
	}
	return(img);
}

void displayBackGroundImage(char *fileName, SDL_Surface *surface) {
	SDL_Surface *img = loadImage (fileName);
	int rgbColor[]={0, 0, 0};
	SDL_Rect bgrect = drawRectangleOnScreen(img->w, img->h, (SCREEN_WIDTH/2)-(img->w/2),(SCREEN_HEIGHT/2)-(img->h/2), rgbColor);
	SDL_BlitSurface(img, NULL, surface, &bgrect);
	SDL_FreeSurface(img);
}

void displayImageOnScreen(char *fileName, char *fallBackText) {
	SDL_Surface *img = loadImage (fileName);
	if (img==NULL) {
		drawImgFallbackTextOnScreen(fallBackText);
	} else {
		SDL_Rect rectangleDest;
		rectangleDest.w = 0;
		rectangleDest.h = 0;
		rectangleDest.x = SCREEN_WIDTH/2-img->w/2;
		rectangleDest.y = ((SCREEN_HEIGHT)/2-img->h/2);
		SDL_BlitSurface(img, NULL, screen, &rectangleDest);
	}
	SDL_FreeSurface(img);
}

void initializeDisplay() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
}

void refreshScreen() {
	SDL_Flip(screen);
}

void initializeFonts() {
	TTF_Init();
	font = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(14));
	loadingFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(18));
	BIGFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(36));
	headerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(16));
}

void freeResources() {
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	SDL_Quit();
	TTF_Quit();
}
