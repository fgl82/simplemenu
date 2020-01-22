#include "../headers/graphics.h"

#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mouse.h>
#ifdef TARGET_RG350
#include <shake.h>
#endif

#include "../headers/definitions.h"
#include "../headers/globals.h"

SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
TTF_Font *picModeFont = NULL;
TTF_Font *BIGFont = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

int calculateProportionalSizeOrDistance(int number) {
	return (SCREEN_HEIGHT*number)/240;
//	return (number*SCREEN_WIDTH)/SCREEN_HEIGHT;
}

int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], SDL_Color txtColor, int align, SDL_Color backgroundColor) {
	SDL_Surface *msg;
	char bufCopy[300];
	strcpy(bufCopy,buf);
	msg = TTF_RenderText_Shaded(font, bufCopy, txtColor, backgroundColor);
	int len=strlen(buf);
	while (msg->w>300) {
		bufCopy[len]='\0';
		SDL_FreeSurface(msg);
		msg = TTF_RenderText_Shaded(font, bufCopy, txtColor, backgroundColor);
		len--;
	}
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

int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], SDL_Color txtColor, int align) {
	SDL_Surface *msg;
	char bufCopy[300];
	strcpy(bufCopy,buf);
	msg = TTF_RenderText_Blended(font, bufCopy, txtColor);
	int len=strlen(buf);
	while (msg->w>calculateProportionalSizeOrDistance(300)) {
		bufCopy[len]='\0';
		SDL_FreeSurface(msg);
		msg = TTF_RenderText_Blended(font, bufCopy, txtColor);
		len--;
	}
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

SDL_Color make_color(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color c= { r, g, b };
	return c;
}

void drawShadedGameNameOnScreen(char *buf, int position) {
	drawShadedTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
}

void drawShadedGameNameOnScreenPicMode(char *buf, int position) {
//	drawShadedTextOnScreen(picModeFont, SCREEN_WIDTH/2, position, buf, make_color(0,0,0), VAlignBottom | HAlignCenter, make_color(255,255,255));
	drawTextOnScreen(headerFont, SCREEN_WIDTH/2, position, buf, make_color(255,255,0), VAlignBottom | HAlignCenter);
}

void drawNonShadedGameNameOnScreen(char *buf, int position) {
	drawTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignCenter);
}

void drawNonShadedGameNameOnScreenPicMode(char *buf, int position) {
	drawTextOnScreen(footerFont, SCREEN_WIDTH/2, position, buf, make_color(255,255,255), VAlignBottom | HAlignCenter);
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
	drawTextOnScreen(footerFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(8), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
}

void drawShutDownText(const char text[64]) {
	SDL_Color white;
	white.r=255;
	white.g=255;
	white.b=255;
	drawTextOnScreen(BIGFont, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text, white, VAlignMiddle | HAlignCenter);
}

void drawTextOnHeader(char *text) {
	drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(24), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignTop | HAlignCenter);
}

void drawTimeOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(316), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignRight);
}

void drawBatteryOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(4), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignLeft);
}

void drawCurrentLetter(char *letter, SDL_Color textColor) {
	drawTextOnScreen(BIGFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), letter, textColor, VAlignMiddle | HAlignCenter);
}

void drawCurrentExecutable(char *executable, SDL_Color textColor) {
	drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), executable, textColor, VAlignMiddle | HAlignCenter);
}

void drawError(char *errorMessage, SDL_Color textColor) {
	if(strchr(errorMessage,'-')==NULL) {
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), errorMessage, textColor, VAlignMiddle | HAlignCenter);
	} else {
		char *line2 = strchr(errorMessage,'-');
		int index = (line2-errorMessage);
		line2++;
		char line1[200];
		strcpy(line1, errorMessage);
		line1[index]='\0';
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)-calculateProportionalSizeOrDistance(12), line1, textColor, VAlignMiddle | HAlignCenter);
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)+calculateProportionalSizeOrDistance(12), line2, textColor, VAlignMiddle | HAlignCenter);
	}
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

void drawUSBScreen() {
	SDL_Color white;
	white.r=255;
	white.g=255;
	white.b=255;
	SDL_Color black;
	black.r=0;
	black.g=0;
	black.b=0;
	displayImageOnScreen("./resources/usb.png","");
	drawTextOnScreen(headerFont,163,29,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,215,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,31,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,217,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,29,"USB MODE",white,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,215,"PRESS START TO END",white,VAlignMiddle | HAlignCenter);
}

void initializeDisplay() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
}

void refreshScreen() {
	SDL_Flip(screen);
}

void initializeFonts() {
	TTF_Init();
	font = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(14));
	picModeFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(15));
	BIGFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(32));
	headerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(16));
}

void freeResources() {
//    pthread_join(clockThread, NULL);
//    pthread_mutex_destroy(&lock);
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	TTF_Quit();
	#ifdef TARGET_RG350
	Shake_Stop(device, effect_id);
	Shake_EraseEffect(device, effect_id);
	Shake_Close(device);
	Shake_Quit();
	#endif
	SDL_Quit();
}
