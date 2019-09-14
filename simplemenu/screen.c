#include <constants.h>
#include <definitions.h>
#include <globals.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
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

void displayGamePicture() {
	int rgbColor[] = {40, 40, 40};
	char gameNameFullPath[200]="";
	if (favoritesSectionSelected) {
		if (favoritesSize == 0) {
			return;
		}
		strcpy(gameNameFullPath,CURRENT_FAVORITE.filesDirectory);
	} else {
		strcpy(gameNameFullPath, CURRENT_SECTION.filesDirectory);
	}
	strcat(gameNameFullPath,removeExtension(CURRENT_GAME_NAME));
	strcat(gameNameFullPath,".png");
//	displayBackGroundImage("./resources/back.png", screen);
	SDL_Color white;
	white.r=255;
	white.g=255;
	white.b=255;

	int backgroundColor[]={CURRENT_SECTION.bodyBackgroundColor.r,CURRENT_SECTION.bodyBackgroundColor.g,CURRENT_SECTION.bodyBackgroundColor.b};
	int footerBackgroundColor[]={CURRENT_SECTION.headerAndFooterTextBackgroundColor.r,CURRENT_SECTION.headerAndFooterTextBackgroundColor.g,CURRENT_SECTION.headerAndFooterTextBackgroundColor.b};

	char nameToDisplay[200]="";
	strcpy(nameToDisplay,CURRENT_GAME_NAME);
	stripGameName(nameToDisplay);

	draw_rectangle(screen, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, backgroundColor);
	displayImageOnSurface(gameNameFullPath, "NO SCREENSHOT", font, CURRENT_SECTION.bodyTextForegroundColor, screen, rgbColor);
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(18), 0, 222, footerBackgroundColor);
	draw_text(screen, font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(239), nameToDisplay, CURRENT_SECTION.headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);


}
void drawHeader() {
	char finalString [100];
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
	if (currentCPU==NO_OC) {
		strcpy(finalString,"- ");
		strcat(finalString,menuSections[currentSectionNumber].sectionName);
		strcat(finalString," -");
	} else if (currentCPU==MED_OC) {
		strcpy(finalString,menuSections[currentSectionNumber].sectionName);
	} else {
		strcpy(finalString,"+ ");
		strcat(finalString,menuSections[currentSectionNumber].sectionName);
		strcat(finalString," +");
	}
	draw_text(screen, headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(24), finalString, menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSectionNumber].bodyBackgroundColor.r,menuSections[currentSectionNumber].bodyBackgroundColor.g,menuSections[currentSectionNumber].bodyBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor);
	int nextLine = calculateProportionalSizeOrDistance(29);
	gamesInPage=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesInPage++;
			char nameWithoutExtension[200];
			strcpy(nameWithoutExtension,gameList[menuSections[currentSectionNumber].currentPage][i]);
			stripGameName(nameWithoutExtension);
			sprintf(buf,"%s", nameWithoutExtension);
			if (i==menuSections[currentSectionNumber].currentGame) {
				draw_shaded_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSectionNumber].bodySelectedTextForegroundColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
			} else {
				draw_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSectionNumber].bodyTextForegroundColor, VAlignBottom | HAlignCenter);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void drawFooter(char *text) {
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	draw_text(screen, font, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), text, menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignMiddle | HAlignCenter);
}

void setupDecorations() {
	drawHeader();
	drawFooter("SELECT+START: SHUT DOWN");
}

void updateScreen() {\
	drawGameList();
	if (pictureMode) {
		displayGamePicture();
	}
	SDL_Flip(screen);
}

void setupDisplay() {
	SDL_PumpEvents();
	keys = SDL_GetKeyState(NULL);
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
	TTF_Init();
	font = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(14));
	headerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(16));
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
