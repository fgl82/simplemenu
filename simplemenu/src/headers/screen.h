#ifndef SCREEN_DEFINED
#define SCREEN_DEFINED
void updateScreen(struct Rom *rom);
void drawHeader();
void drawGameList();
void drawFooter(char *text);
void setupDisplayAndKeys();
void setupDecorations();
void freeResources();
void displayGamePicture();
void displayBackgroundPicture();
void showConsole();
void drawShutDownScreen();
uint32_t hideFullScreenModeMenu();
void resetPicModeHideMenuTimer();
void resetPicModeHideLogoTimer();
void clearPicModeHideMenuTimer();
void clearPicModeHideLogoTimer();
void clearHideHeartTimer();
void resetHideHeartTimer();
#endif
