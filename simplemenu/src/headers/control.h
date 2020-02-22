#ifndef CONTROL_DEFINED
#define CONTROL_DEFINED
#include <SDL/SDL_events.h>
#include "../headers/globals.h"
int advanceSection();
int rewindSection();
void showPicture();
void launchGame();
void launchEmulator();
void scrollUp();
void scrollDown();
void scrollToGame(int gameNumber);
void advancePage();
void rewindPage();
void showOrHideFavorites();
void removeFavorite();
void markAsFavorite();
int isSelectPressed();
void performChoosingAction();
int performAction();
void callDeleteGame(struct Rom *rom);
#endif
