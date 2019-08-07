#include <constants.h>
#include <globals.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string_utils.h>
#include <definitions.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

SDL_Surface *screen = NULL;
char buf[1024];
TTF_Font *font = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

void drawHeader() {
	int rgbColor[] = {menuSections[currentSection].headerAndFooterTextBackgroundColor.r,menuSections[currentSection].headerAndFooterTextBackgroundColor.g,menuSections[currentSection].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
	draw_text(screen, headerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(23), menuSections[currentSection].sectionName, menuSections[currentSection].headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSection].bodyBackgroundColor.r,menuSections[currentSection].bodyBackgroundColor.g,menuSections[currentSection].bodyBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor);
	int nextLine = calculateProportionalSizeOrDistance(29);
	gamesInPage=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (gameList[currentPage][i]!=NULL) {
			gamesInPage++;
			char *nameWithoutExtension = removeExtension(gameList[currentPage][i]);
			int charNumber = 0;
			while (nameWithoutExtension[charNumber]) {
				if (nameWithoutExtension[charNumber]=='('||charNumber>37) {
					nameWithoutExtension[charNumber]='\0';
					break;
				}
				charNumber++;
			}
			sprintf(buf,"%s", nameWithoutExtension);
			free(nameWithoutExtension);
			if (i==currentGame) {
				draw_shaded_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSection].bodySelectedTextForegroundColor, VAlignBottom | HAlignCenter, menuSections[currentSection].bodySelectedTextBackgroundColor);
			} else {
				draw_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSection].bodyTextForegroundColor, VAlignBottom | HAlignCenter);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void updateScreen() {
	drawGameList();
	SDL_Flip(screen);
}

void drawFooter() {
	int rgbColor[] = {menuSections[currentSection].headerAndFooterTextBackgroundColor.r,menuSections[currentSection].headerAndFooterTextBackgroundColor.g,menuSections[currentSection].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	draw_text(screen, font, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), "SELECT+START: SHUT DOWN", menuSections[currentSection].headerAndFooterTextForegroundColor, VAlignMiddle | HAlignCenter);
}

void setupDisplay() {
	keys = SDL_GetKeyState(NULL);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	SDL_PumpEvents();
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
	TTF_Init();
	font = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(14));
	headerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(16));
}

void setupDecorations() {
	drawHeader();
	drawFooter();
}

void freeResources() {
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	SDL_Quit();
	TTF_Quit();
}
