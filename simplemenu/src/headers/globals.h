#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <SDL/SDL_events.h>
#include <SDL/SDL_video.h>
int running;
int currentSectionNumber;
int gamesInPage;
int totalPages;
int favoritesSectionNumber;
int favoritesSize;
int menuSectionCounter;
int currentCPU;
int favoritesSectionSelected;
int favoritesChanged;
int returnTo;
char *gameList[1000][10];
int pictureMode;
int hotKeyPressed;

struct MenuSection {
	char sectionName[25];
	char emulatorFolder[25];
	char executable[16];
	char filesDirectory[25];
	char fileExtensions[50];
	int hidden;
	int currentPage;
	int currentGame;
	int alphabeticalPaging;
	SDL_Color headerAndFooterTextBackgroundColor;
	SDL_Color headerAndFooterTextForegroundColor;
	SDL_Color bodyTextForegroundColor;
	SDL_Color bodySelectedTextBackgroundColor;
	SDL_Color bodySelectedTextForegroundColor;
	SDL_Color bodyBackgroundColor;
};
struct MenuSection menuSections[50];

struct Favorite {
	char name[100];
	char emulatorFolder[25];
	char executable[16];
	char filesDirectory[25];
};

struct Favorite favorites[2000];

uint8_t *keys;
#endif
