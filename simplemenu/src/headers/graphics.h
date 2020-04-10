#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include <SDL/SDL_ttf.h>

static const int
HAlignLeft = 1,
HAlignRight = 2,
HAlignCenter = 4,
VAlignTop = 8,
VAlignBottom = 16,
VAlignMiddle = 32;
int calculateProportionalSizeOrDistance(int number);
int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align, int backgroundColor[]);
int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align);
void drawShadedGameNameOnScreen(char *buf, int position);
void drawNonShadedGameNameOnScreen(char *buf, int position);
void drawPictureTextOnScreen(char *buf);
void drawImgFallbackTextOnScreen( char *fallBackText);
void drawTextOnFooter(const char text[64]);
void drawTextOnHeader(char *text);
void drawCurrentLetter(char *letter, int textColor[], int x, int y);
SDL_Rect drawRectangleOnScreen(int width, int height, int x, int y, int rgbColor[]);
void drawTransparentRectangleToScreen(int w, int h, int x, int y, int rgbColor[], int transparency);
SDL_Surface *loadImage (char *fileName);
void displayBackGroundImage(char *fileName, SDL_Surface *surface);
void displayImageOnScreen(char *fileName, char *fallBackText);
void initializeDisplay();
void refreshScreen();
void initializeFonts();
void freeResources();
void drawUSBScreen();
void drawShutDownText();
void drawError(char *errorMessage, int textColor[]);
void drawCurrentExecutable(char *executable, int textColor[]);
void drawCurrentSectionGroup(char *groupName, int textColor[]);
void drawShadedGameNameOnScreenPicMode(char *buf, int position);
void drawNonShadedGameNameOnScreenPicMode(char *buf, int position);
void drawTextOnHeaderWithColor(char *text, int txtColor[]);
void drawShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[], int txtBackgroundColor[]);
void drawNonShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[]);
void drawTextOnFooterWithColor(const char text[64], int txtColor[]);
#endif
