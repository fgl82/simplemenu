#include <config.h>
#include <definitions.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stdlib.h>
#include <string.h>
#include <system_logic.h>
#include <SDL/SDL_events.h>
#include <unistd.h>

void advanceSection() {
	currentSection++;
	setupDecorations();
	totalPages=0;
	loadGameList();
}

void rewindSection() {
	currentSection--;
	setupDecorations();
	totalPages=0;
	loadGameList();
}

void launchGame() {
	char fileToBeExecutedwithFullPath[200];
	if (gameList[menuSections[currentSection].currentPage][menuSections[currentSection].currentGame]!=NULL) {
		strcpy(fileToBeExecutedwithFullPath,menuSections[currentSection].filesDirectory);
		strcat(fileToBeExecutedwithFullPath,gameList[menuSections[currentSection].currentPage][menuSections[currentSection].currentGame]);
		executeCommand(menuSections[currentSection].executable,fileToBeExecutedwithFullPath);
	}
}

void scrollDown() {
	if(menuSections[currentSection].currentGame == gamesInPage-1) {
		if (menuSections[currentSection].currentPage < totalPages) {
			menuSections[currentSection].currentGame=0;
			menuSections[currentSection].currentPage++;
			return;
		}
	}
	if (menuSections[currentSection].currentGame < gamesInPage-1) {
		menuSections[currentSection].currentGame++;
		return;
	}
}

void scrollUp() {
	if(menuSections[currentSection].currentGame == 0) {
		if (menuSections[currentSection].currentPage >0) {
			menuSections[currentSection].currentGame=gamesInPage-1;
			menuSections[currentSection].currentPage--;
			return;
		}
	}
	if (menuSections[currentSection].currentGame > 0) {
		menuSections[currentSection].currentGame--;
		return;
	}
}

void advancePage() {
	if (menuSections[currentSection].currentPage < totalPages) {
		menuSections[currentSection].currentGame=0;
		menuSections[currentSection].currentPage++;
	}
}

void rewindPage() {
	if (menuSections[currentSection].currentPage > 0) {
		menuSections[currentSection].currentGame=0;
		menuSections[currentSection].currentPage--;
	}
}

int performAction(SDL_Event event) {
	if (keys[BTN_SELECT] && keys[BTN_START]) {
		running=0;
		return 0;
	}
	if (keys[BTN_TA] && keys[BTN_START]) {
		freeResources();
		saveLastState();
		exit(0);
	}
	if(keys[BTN_TB]) {
		cycleFrequencies();
		drawHeader();
		return 0;
	}
	if(keys[BTN_TA]) {
		if(strcmp(menuSections[currentSection+1].sectionName,"END")!=0) {
			advanceSection();
			return 0;
		}
	}
	if(keys[BTN_B]) {
		if(currentSection>0) {
			rewindSection();
			return 0;
		}
	}
	if (keys[BTN_A]) {
		launchGame();
		return 0;
	}
	if (keys[BTN_DOWN]) {
		scrollDown();
		return 1;
	}
	if(keys[BTN_UP]) {
		scrollUp();
		return 1;
	}
	if(keys[BTN_RIGHT]) {
		advancePage();
		return 1;
	}
	if(keys[BTN_LEFT]) {
		rewindPage();
		return 1;
	}
	return 0;
}
