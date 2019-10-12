#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <SDL/SDL_events.h>
#include <SDL/SDL_video.h>
int running;

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

struct MenuSection {
	char sectionName[25];
	char emulatorFolder[200];
	char executable[200];
	char filesDirectory[200];
	char fileExtensions[50];
	char consolePicture[100];
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
	char *gameList[1000][10];
};
struct MenuSection menuSections[50];

struct Favorite {
	char name[100];
	char emulatorFolder[200];
	char executable[200];
	char filesDirectory[200];
    struct Favorite *next;
};

struct Favorite favorites[2000];

uint8_t *keys;
#endif
