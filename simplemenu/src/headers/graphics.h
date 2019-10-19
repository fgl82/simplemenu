#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

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
int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align, SDL_Color backgroundColor);
int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[64], SDL_Color txtColor, int align);
void drawShadedGameNameOnScreen(char *buf, int position);
void drawNonShadedGameNameOnScreen(char *buf, int position);
void drawPictureTextOnScreen(char *buf);
void drawImgFallbackTextOnScreen( char *fallBackText);
void drawTextOnFooter(const char text[64]);
void drawTextOnHeader(char *text);
void drawCurrentLetter(char *letter, SDL_Color textColor);
SDL_Rect drawRectangleOnScreen(int width, int height, int x, int y, int rgbColor[]);
SDL_Surface *loadImage (char *fileName);
void displayBackGroundImage(char *fileName, SDL_Surface *surface);
void displayImageOnScreen(char *fileName, char *fallBackText);
void initializeDisplay();
void refreshScreen();
void initializeFonts();
void freeResources();
void drawUSBScreen();
#endif
