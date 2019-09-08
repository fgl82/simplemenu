#include <constants.h>
#include <definitions.h>
#include <globals.h>
#include <graphics.h>
#include <string_utils.h>
#include <string.h>

SDL_Surface *screen = NULL;
char buf[1024];
TTF_Font *font = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

void displayGamePicture() {
	char gameNameFullPath[100];
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
	displayImageOnSurface(gameNameFullPath, screen);
}
void drawHeader() {
	char finalString [100];
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor, 0);
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
	draw_text(screen, headerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(23), finalString, menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSectionNumber].bodyBackgroundColor.r,menuSections[currentSectionNumber].bodyBackgroundColor.g,menuSections[currentSectionNumber].bodyBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor, 0);
	int nextLine = calculateProportionalSizeOrDistance(29);
	gamesInPage=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesInPage++;
			char *nameWithoutExtension = removeExtension(gameList[menuSections[currentSectionNumber].currentPage][i]);
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
			if (i==menuSections[currentSectionNumber].currentGame) {
				draw_shaded_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSectionNumber].bodySelectedTextForegroundColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
			} else {
				draw_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSectionNumber].bodyTextForegroundColor, VAlignBottom | HAlignCenter);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void drawFooter() {
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor, 0);
	draw_text(screen, font, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), "SELECT+START: SHUT DOWN", menuSections[currentSectionNumber].headerAndFooterTextForegroundColor, VAlignMiddle | HAlignCenter);
}

void setupDecorations() {
	drawHeader();
	drawFooter();
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
