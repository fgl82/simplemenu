#ifndef SCREEN_DEFINED
#define SCREEN_DEFINED
void updateScreen();
void drawHeader();
void drawGameList();
void drawFooter();
void setupDisplayAndKeys();
void setupDecorations();
void freeResources();
void displayGamePicture();
void displayBackgroundPicture();
void showConsole();
void drawShutDownScreen();
uint32_t hidePicModeMenu();
void resetPicModeHideMenuTimer();
void resetPicModeHideLogoTimer();
#endif
