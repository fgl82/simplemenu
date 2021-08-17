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
int drawShadedTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int backgroundColor[]);
int drawTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align);
void drawShadedGameNameOnScreen(char *buf, int position);
void drawNonShadedGameNameOnScreen(char *buf, int position);
void drawPictureTextOnScreen(char *buf);
void drawImgFallbackTextOnScreen( char *fallBackText);
void drawTextOnFooter(char *text);
void drawTextOnHeader();
void drawCurrentLetter(char *letter, int textColor[], int x, int y);
SDL_Rect drawRectangleToScreen(int width, int height, int x, int y, int rgbColor[]);
void drawTransparentRectangleToScreen(int w, int h, int x, int y, int rgbColor[], int transparency);
SDL_Surface *loadImage (char *fileName);
void displayBackGroundImage(char *fileName, SDL_Surface *surface);
void displayCenteredImageOnScreen(char *fileName, char *fallBackText, int scaleToFullScreen, int keepRatio);
void initializeDisplay();
void refreshScreen();
void initializeFonts();
void freeResources();
void drawUSBScreen();
void drawBigWhiteText();
void drawError(char *errorMessage, int textColor[]);
void drawCurrentExecutable(char *executable, int textColor[]);
void drawCurrentSectionGroup(char *groupName, int textColor[]);
void drawShadedGameNameOnScreenPicMode(char *buf, int position);
void drawNonShadedGameNameOnScreenPicMode(char *buf, int position);
void drawTextOnHeaderWithColor(char *text, int txtColor[]);
void drawShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[], int txtBackgroundColor[]);
void drawNonShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[]);
void drawTextOnFooterWithColor(char *text, int txtColor[]);
void displayImageOnScreenTraditional(char *fileName);
void displayImageOnScreenDrunkenMonkey(char *fileName);
void freeFonts();
void freeSettingsFonts();
void initializeSettingsFonts();
void drawTextOnSettingsFooterWithColor(char *text, int txtColor[]);
void drawTextOnSettingsHeaderWithColor(char *text, int txtColor[]);
void drawShadedGameNameOnScreenLeft(char *buf, int position);
void drawNonShadedGameNameOnScreenLeft(char *buf, int position);
void drawShadedGameNameOnScreenCenter(char *buf, int position);
void drawNonShadedGameNameOnScreenCenter(char *buf, int position);
void displayHeart();
void* thread_func(void *picture);
void genericDrawMultiLineTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int maxWidth, int lineSeparation);
void resizeGroupBackground(struct SectionGroup *group);
void displayImageOnScreenCustom(char *fileName);
void drawShadedGameNameOnScreenCustom(char *buf, int position);
void drawNonShadedGameNameOnScreenCustom(char *buf, int position);
void displayCenteredSurface(SDL_Surface *surface);
void resizeSectionBackground(struct MenuSection *section);
void resizeSectionSystemLogo(struct MenuSection *section);
void drawCustomGameNumber(char *buf, int x, int y);
void displaySurface(SDL_Surface *surface, int x, int y);
void resizeSectionSystemPicture(struct MenuSection *section);
void *updateClock(void *x_void_ptr);
void drawTextOnSettingsHeaderLeftWithColor(char *text, int txtColor[]);
void drawTextOnSettingsHeaderRightWithColor(char *text, int txtColor[]);
void drawLoadingText();
void drawShadedSettingsOptionValueOnScreen(char *option, char *value, int position, int txtColor[], int txtBackgroundColor[]);
void drawSettingsOptionValueOnScreen(char *value, int position, int txtColor[]);
int genericDrawTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int backgroundColor[], int shaded);
void drawCopyingText(char* text);
int drawImage(SDL_Surface* display, SDL_Surface *image, int x, int y, int xx, int yy , const double newwidth, const double newheight, int transparent, int smoothing);
TTF_Font *getFont();
TTF_Font *getFooterFont();
TTF_Font *getSettingsFont();
TTF_Font *getBigFont();
TTF_Font *getGameCountFont();
#endif
