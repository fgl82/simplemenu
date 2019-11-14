#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

#include <SDL/SDL_joystick.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_video.h>
#include "../headers/hashtable.h"


int running;

int itsStoppedBecauseOfAnError;
char *errorMessage;
int thereIsACriticalError;

int favoritesSectionNumber;
int favoritesSectionSelected;
int favoritesChanged;
int favoritesSize;
int returnTo;

int currentSectionNumber;
int menuSectionCounter;
int gamesInPage;

int currentCPU;
int pictureMode;
int hotKeyPressed;
int leftOrRightPressed;

int OC_UC;
int OC_NO;
int OC_OC;
int OC_SLEEP;

SDL_TimerID timeoutTimer;
int timeoutValue;
int isSuspended;
int isUSBMode;
int backlightValue;

char logLine[500];

struct MenuSection {
	char sectionName[25];
	char emulatorFolder[200];
	char executable[200];
	char filesDirectory[200];
	char fileExtensions[50];
	char consolePicture[100];
	char aliasFileName[300];
	int hidden;
	int currentPage;
	int currentGame;
	int alphabeticalPaging;
	int totalPages;
	SDL_Color headerAndFooterTextBackgroundColor;
	SDL_Color headerAndFooterTextForegroundColor;
	SDL_Color bodyTextForegroundColor;
	SDL_Color bodySelectedTextBackgroundColor;
	SDL_Color bodySelectedTextForegroundColor;
	SDL_Color bodyBackgroundColor;
	struct Rom *romList[1000][10];
	hashtable_t *aliasHashTable;
};
struct MenuSection menuSections[50];

struct Favorite {
	char name[300];
	char alias[300];
	char emulatorFolder[200];
	char executable[200];
	char filesDirectory[200];
};

struct Rom {
	char *name;
	char *alias;
};

struct Favorite favorites[2000];

uint8_t *keys;
SDL_Joystick *joystick;
#endif
