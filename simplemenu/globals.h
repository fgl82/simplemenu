#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <SDL/SDL_events.h>
#include <SDL/SDL_video.h>
int running;
SDL_Event event;
int currentSection;
int gamesInPage;
int totalPages;
int currentCPU;
char *gameList[200][10];
struct MenuSection {
	char sectionName[16];
	char emulatorFolder[24];
	char executable[16];
	char filesDirectory[21];
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
struct MenuSection menuSections[15];
uint8_t *keys;
#endif
