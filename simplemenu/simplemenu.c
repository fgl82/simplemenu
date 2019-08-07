#include <bits/types/FILE.h>
#include <control.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <unistd.h>

void initializeGlobals() {
	running=1;
	currentGame = 0;
	currentPage = 0;
	currentSection = 0;
	gamesInPage=0;
	totalPages=0;
}

void readConfig() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	struct MenuSection aMenuSection;
	fp = fopen("./config/sections.cfg", "r");
	int menuSectionCounter = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		char *ptr = strtok(line, ";");
		char *configurations[21];
		int i=0;
		while(ptr != NULL)
		{
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		strcpy(aMenuSection.sectionName,configurations[0]);
		strcpy(aMenuSection.executable,configurations[1]);
		strcpy(aMenuSection.filesDirectory,configurations[2]);
		aMenuSection.headerAndFooterTextBackgroundColor.r=atoi(configurations[3]);
		aMenuSection.headerAndFooterTextBackgroundColor.g=atoi(configurations[4]);
		aMenuSection.headerAndFooterTextBackgroundColor.b=atoi(configurations[5]);
		aMenuSection.headerAndFooterTextForegroundColor.r=atoi(configurations[6]);
		aMenuSection.headerAndFooterTextForegroundColor.g=atoi(configurations[7]);
		aMenuSection.headerAndFooterTextForegroundColor.b=atoi(configurations[8]);
		aMenuSection.bodyBackgroundColor.r=atoi(configurations[9]);
		aMenuSection.bodyBackgroundColor.g=atoi(configurations[10]);
		aMenuSection.bodyBackgroundColor.b=atoi(configurations[11]);
		aMenuSection.bodyTextForegroundColor.r=atoi(configurations[12]);
		aMenuSection.bodyTextForegroundColor.g=atoi(configurations[13]);
		aMenuSection.bodyTextForegroundColor.b=atoi(configurations[14]);
		aMenuSection.bodySelectedTextBackgroundColor.r=atoi(configurations[15]);
		aMenuSection.bodySelectedTextBackgroundColor.g=atoi(configurations[16]);
		aMenuSection.bodySelectedTextBackgroundColor.b=atoi(configurations[17]);
		aMenuSection.bodySelectedTextForegroundColor.r=atoi(configurations[18]);
		aMenuSection.bodySelectedTextForegroundColor.g=atoi(configurations[19]);
		aMenuSection.bodySelectedTextForegroundColor.b=atoi(configurations[20]);
		menuSections[menuSectionCounter]=aMenuSection;
		menuSectionCounter++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
}

int main(int argc, char *argv[]) {
	readConfig();
	initializeGlobals();
	setupDisplay();
	setupDecorations();
	loadGameList();
	updateScreen();
	while (running) {
		SDL_EnableKeyRepeat(30,180);
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				performAction(event);
				updateScreen();
			}
			if (event.type == SDL_KEYUP) {
				break;
			}
		}
	}
	quit();
}
