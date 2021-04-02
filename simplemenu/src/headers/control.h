#ifndef CONTROL_DEFINED
#define CONTROL_DEFINED
#include <SDL/SDL_events.h>
#include "../headers/globals.h"
int advanceSection(int showLogo);
int rewindSection(int showLogo);
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
void performGroupChoosingAction();
void performSettingsChoosingAction();
int performAction();
void callDeleteGame(struct Node *node);
#endif
