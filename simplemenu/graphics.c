#include "graphics.h"
#include "definitions.h"

int calculateProportionalSizeOrDistance(int number) {
	return (SCREEN_HEIGHT*number)/240;
}

int draw_shaded_text(SDL_Surface *destinationSurface, TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align, SDL_Color backgroundColor) {
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
	SDL_BlitSurface(msg, NULL, destinationSurface, &rect);
	SDL_FreeSurface(msg);
	return msg->w;
}

int draw_text(SDL_Surface *destinationSurface, TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align) {
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
	SDL_BlitSurface(msg, NULL, destinationSurface, &rect);
	SDL_FreeSurface(msg);
	return msg->w;
}

SDL_Rect draw_rectangle(SDL_Surface *surface, int width, int height, int x, int y, int rgbColor[], int isBackground) {
	SDL_Rect rectangle;
	rectangle.w = width;
	rectangle.h = height;
	rectangle.x = x;
	rectangle.y = y;
	if (isBackground) {
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	} else {
		SDL_FillRect(surface, &rectangle, SDL_MapRGB(surface->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	}
	return(rectangle);
}

void displayImageOnSurface(char *fileName, SDL_Surface *surface, int rgbColor[]) {
	SDL_Surface* img = NULL;
	SDL_Surface* _img = IMG_Load(fileName);
	if (_img==NULL) {
		_img = IMG_Load("./resources/default.png");
	}
	img = SDL_DisplayFormatAlpha(_img);
	SDL_FreeSurface(_img);
	SDL_Rect bgrect = draw_rectangle(surface, (SCREEN_WIDTH/2)-(img->w/2), img->h, 0, 0, rgbColor, 1);
	SDL_BlitSurface(img, NULL, surface, &bgrect);
}
