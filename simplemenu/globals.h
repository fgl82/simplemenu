#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <SDL/SDL_events.h>
#include <SDL/SDL_video.h>
int running;
SDL_Event event;
int currentSectionNumber;
int gamesInPage;
int totalPages;
int lastSection;
int favoritesSize;
int currentCPU;
int favoritesSectionSelected;
int favoritesChanged;
char *gameList[200][10];

struct MenuSection {
	char sectionName[25];
	char emulatorFolder[25];
	char executable[16];
	char filesDirectory[25];
	char fileExtension[5];
	int currentPage;
	int currentGame;
	SDL_Color headerAndFooterTextBackgroundColor;
	SDL_Color headerAndFooterTextForegroundColor;
	SDL_Color bodyTextForegroundColor;
	SDL_Color bodySelectedTextBackgroundColor;
	SDL_Color bodySelectedTextForegroundColor;
	SDL_Color bodyBackgroundColor;
};
struct MenuSection menuSections[20];

struct Favorite {
	char name[100];
	char emulatorFolder[25];
	char executable[16];
	char filesDirectory[25];
};

struct Favorite favorites[100];

uint8_t *keys;
#endif
