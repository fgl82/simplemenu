#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

static const int
HAlignLeft = 1,
HAlignRight = 2,
HAlignCenter = 4,
VAlignTop = 8,
VAlignBottom = 16,
VAlignMiddle = 32;
int calculateProportionalSizeOrDistance(int number);
int draw_shaded_text(SDL_Surface *destinationSurface, TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align, SDL_Color backgroundColor);
int draw_text(SDL_Surface *destinationSurface, TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align);
SDL_Rect draw_rectangle(SDL_Surface *surface, int width, int height, int x, int y, int rgbColor[], int isBackground);
void displayImageOnSurface(char *image, SDL_Surface *surface);
#endif
