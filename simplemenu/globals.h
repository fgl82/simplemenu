#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <SDL/SDL_events.h>
#include <SDL/SDL_video.h>
int running;
SDL_Event event;
int currentGame;
int currentPage;
int currentSection;
int gamesInPage;
int totalPages;
char *gameList[200][10];
struct MenuSection {
	char sectionName[50];
	char executable[100];
	char filesDirectory[100];
	SDL_Color headerAndFooterTextBackgroundColor;
	SDL_Color headerAndFooterTextForegroundColor;
	SDL_Color bodyTextForegroundColor;
	SDL_Color bodySelectedTextBackgroundColor;
	SDL_Color bodySelectedTextForegroundColor;
	SDL_Color bodyBackgroundColor;
};
struct MenuSection menuSections[25];
uint8_t *keys;
#endif
