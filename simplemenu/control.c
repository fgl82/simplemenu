#include <definitions.h>
#include <globals.h>
#include <system_logic.h>
#include <graphics.h>
#include <logic.h>
#include <screen.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_events.h>

void advanceSection() {
	currentSection++;
	setupDecorations();
	totalPages=0;
	loadGameList();
	currentGame=0;
	currentPage=0;
}

void rewindSection() {
	currentSection--;
	setupDecorations();
	totalPages=0;
	loadGameList();
	currentGame=0;
	currentPage=0;
}

void launchGame() {
	char fileToBeExecutedwithFullPath[200];
	if (gameList[currentPage][currentGame]!=NULL) {
		strcpy(fileToBeExecutedwithFullPath,menuSections[currentSection].filesDirectory);
		strcat(fileToBeExecutedwithFullPath,gameList[currentPage][currentGame]);
		if (currentSection>0) {
			executeCommand(menuSections[currentSection].executable,fileToBeExecutedwithFullPath);
		} else {
			executeFavorite(fileToBeExecutedwithFullPath);
		}
	}
}

void scrollDown() {
	if(currentGame == gamesInPage-1) {
		if (currentPage < totalPages) {
			currentGame=0;
			currentPage++;
			return;
		}
	}
	if (currentGame < gamesInPage-1) {
		currentGame++;
		return;
	}
}

void scrollUp() {
	if(currentGame == 0) {
		if (currentPage>0) {
			currentGame=gamesInPage-1;
			currentPage--;
			return;
		}
	}
	if (currentGame > 0) {
		currentGame--;
		return;
	}
}

void advancePage() {
	if (currentPage < totalPages) {
		currentGame=0;
		currentPage++;
	}
}

void rewindPage() {
	if (currentPage > 0) {
		currentGame=0;
		currentPage--;
	}
}

void performAction(SDL_Event event) {
	if (keys[BTN_SELECT] && keys[BTN_START]) {
		running=0;
		return;
	}
	if (keys[BTN_TA] && keys[BTN_START]) {
		freeResources();
		exit(0);
	}
	if(keys[BTN_TB]) {
		cycleFrequencies();
		drawHeader();
		return;
	}
	if(keys[BTN_TA]) {
		if(strcmp(menuSections[currentSection+1].sectionName,"END")!=0) {
			advanceSection();
			return;
		}
	}
	if(keys[BTN_B]) {
		if(currentSection>0) {
			rewindSection();
			return;
		}
	}
	if (keys[BTN_A]) {
		launchGame();
		return;
	}
	if (keys[BTN_DOWN]) {
		scrollDown();
		return;
	}
	if(keys[BTN_UP]) {
		scrollUp();
		return;
	}
	if(keys[BTN_RIGHT]) {
		advancePage();
		return;
	}
	if(keys[BTN_LEFT]) {
		rewindPage();
		return;
	}

}
