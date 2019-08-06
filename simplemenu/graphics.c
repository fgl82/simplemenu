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

void draw_rectangle(SDL_Surface *surface, int width, int height, int x, int y, int rgbColor[]) {
	SDL_Rect rectangle;
	rectangle.w = width;
	rectangle.h = height;
	rectangle.x = x;
	rectangle.y = y;
	SDL_FillRect(surface, &rectangle, SDL_MapRGB(surface->format, rgbColor[0], rgbColor[1], rgbColor[2]));
}
