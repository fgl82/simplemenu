#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_video.h>

#include "../headers/constants.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/string_utils.h"

char buf[1024];

void displayLoadingSign() {
	int width = 90;
	int height = 30;
	int filling[3];
	int borderColor[3];
	borderColor[0]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.r+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.r+45;
	borderColor[1]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.g+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.g+45;
	borderColor[2]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.b+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.b+45;
	filling[0]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.r;
	filling[1]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.g;
	filling[2]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.b;
	SDL_Color textColor = CURRENT_SECTION.headerAndFooterTextForegroundColor;
	if (pictureMode) {

		filling[0] = 21;
		filling[1] = 18;
		filling[2] = 26;
		borderColor[0]=255;
		borderColor[1]=255;
		borderColor[2]=255;
		textColor.r=255;
		textColor.g=255;
		textColor.b=255;
	}
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width), calculateProportionalSizeOrDistance(height), SCREEN_WIDTH/2-(width/2),SCREEN_HEIGHT/2-(height/2), borderColor);
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width), calculateProportionalSizeOrDistance(height), SCREEN_WIDTH/2-(width/2),SCREEN_HEIGHT/2-(height/2), filling);
	char *loading = malloc(strlen("LOADING")+1);
	strcpy(loading,"LOADING");
	drawLoadingSign(loading, textColor);
	free(loading);
}

void showLetter() {
	int width = 80;
	int filling[3];
	int borderColor[3];
	borderColor[0]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.r+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.r+45;
	borderColor[1]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.g+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.g+45;
	borderColor[2]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.b+45>255?255:CURRENT_SECTION.headerAndFooterTextBackgroundColor.b+45;
	filling[0]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.r;
	filling[1]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.g;
	filling[2]=CURRENT_SECTION.headerAndFooterTextBackgroundColor.b;
	SDL_Color textColor = CURRENT_SECTION.headerAndFooterTextForegroundColor;
	if (pictureMode) {
		filling[0] = 21;
		filling[1] = 18;
		filling[2] = 26;
		borderColor[0]=255;
		borderColor[1]=255;
		borderColor[2]=255;
		textColor.r=255;
		textColor.g=255;
		textColor.b=255;
	}
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width+10), calculateProportionalSizeOrDistance(width+10), SCREEN_WIDTH/2-width/2-5,SCREEN_HEIGHT/2-width/2-5, borderColor);
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width), calculateProportionalSizeOrDistance(width), SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2-width/2, filling);
	char letter[2]="";
	char *currentGame = malloc(strlen(CURRENT_GAME_NAME)+1);
	strcpy(currentGame, CURRENT_GAME_NAME);
	stripGameName(currentGame);
	letter[0]=toupper(currentGame[0]);
	letter[1]='\0';
	if(isdigit(letter[0])) {
		letter[0]='#';
	}

	drawCurrentLetter(letter, textColor);
	free(currentGame);
}


void showConsole() {
	int rgbColor[] = {200, 200, 200};
//	drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, rgbColor);
	displayImageOnScreen(CURRENT_SECTION.consolePicture, "PICTURE NOT FOUND");
}

void displayGamePicture() {
	int rgbColor[] = {21, 18, 26};
	char gameNameFullPath[200]="";
	if (favoritesSectionSelected) {
		if (favoritesSize == 0) {
			return;
		}
		struct Favorite favorite = findFavorite(CURRENT_GAME_NAME);
		strcpy(gameNameFullPath, removeExtension(favorite.name));
	} else {
		strcpy(gameNameFullPath, removeExtension(CURRENT_GAME_NAME));
	}
	//	strcat(gameNameFullPath,removeExtension(CURRENT_GAME_NAME));
	strcat(gameNameFullPath,".png");
	//	displayBackGroundImage("./resources/back.png", screen);

	char nameToDisplay[200]="";
	strcpy(nameToDisplay,CURRENT_GAME_NAME);
	stripGameName(nameToDisplay);

	drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, rgbColor);
	displayImageOnScreen(gameNameFullPath, "NO SCREENSHOT");
	drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(18), 0, 222, rgbColor);
	drawPictureTextOnScreen(nameToDisplay);
}

void displayBackgroundPicture() {
	displayImageOnScreen("./resources/back.png", "NO SCREENSHOT");
}

void drawHeader() {
	char finalString [100];
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
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
	drawTextOnHeader(finalString);
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSectionNumber].bodyBackgroundColor.r,menuSections[currentSectionNumber].bodyBackgroundColor.g,menuSections[currentSectionNumber].bodyBackgroundColor.b};
	drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor);
	gamesInPage=0;
	int nextLine = calculateProportionalSizeOrDistance(29);
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesInPage++;
			char nameWithoutExtension[200];
			strcpy(nameWithoutExtension,CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][i]);
			stripGameName(nameWithoutExtension);
			sprintf(buf,"%s", "");
			sprintf(buf,"%s", nameWithoutExtension);
			if (i==menuSections[currentSectionNumber].currentGame) {
				drawShadedGameNameOnScreen(buf, nextLine);
			} else {
				drawNonShadedGameNameOnScreen(buf, nextLine);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void drawFooter(char *text) {
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.r,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.g,menuSections[currentSectionNumber].headerAndFooterTextBackgroundColor.b};
	drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	drawTextOnFooter(text);
}

void setupDecorations() {
	drawHeader();
	char tempString[200];
	if (CURRENT_GAME_NAME==NULL) {
		snprintf(tempString,sizeof(tempString),"GAME %d of %d",CURRENT_SECTION.currentGame+10*CURRENT_SECTION.currentPage, countGamesInSection());
	} else {
		snprintf(tempString,sizeof(tempString),"GAME %d of %d",CURRENT_SECTION.currentGame+1+10*CURRENT_SECTION.currentPage, countGamesInSection());
	}
	drawFooter(tempString);
//	displayLoadingSign();

}

void updateScreen() {
	drawGameList();
	setupDecorations();
	if (pictureMode) {
		displayGamePicture();
	}
	if (hotKeyPressed) {
		showLetter();
	}
	if (leftOrRightPressed) {
		displayBackgroundPicture();
		showConsole();
	}
	refreshScreen();
}

void setupDisplay() {
	pumpEvents();
	initializeKeys();
	initializeFonts();
	initializeDisplay();
}
