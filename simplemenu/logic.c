#include <constants.h>
#include <dirent.h>
#include <globals.h>
#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mouse.h>
#include <unistd.h>

void quit() {
	freeResources();
	execlp("sh", "sh", "-c", "kill $(ps -al | grep \"/mnt/\" | grep -v \"/kernel/\" | tr -s [:blank:] | cut -d \" \" -f 2) ; sleep 0.1 ; sync && poweroff",  NULL);
}

char *determineExecutable(char *fileToBeExecutedWithFullPath) {
	char *ext = getExtension(fileToBeExecutedWithFullPath);
	if (strcmp(ext,".gb")==0) {
		return menuSections[1].executable;
	}
	if (strcmp(ext,".gbc")==0) {
		return menuSections[2].executable;
	}
	if (strcmp(ext,".gba")==0) {
		return menuSections[3].executable;
	}
	if (strcmp(ext,".nes")==0) {
		return menuSections[4].executable;
	}
	if (strcmp(ext,".sfc")==0) {
		return menuSections[5].executable;
	}
	if (strcmp(ext,".gg")==0) {
		return menuSections[6].executable;
	}
	if (strcmp(ext,".sms")==0) {
		return menuSections[7].executable;
	}
	if (strcmp(ext,".md")==0) {
		return menuSections[8].executable;
	}
	if (strcmp(ext,".iso")==0) {
		return menuSections[9].executable;
	}
	if (strcmp(ext,".zip")==0) {
		return menuSections[10].executable;
	}
	if (strcmp(ext,".pce")==0) {
		return menuSections[11].executable;
	}
	if (strcmp(ext,".bin")==0) {
		return menuSections[12].executable;
	}
	return NULL;
}

void executeFavorite (char fileToBeExecutedWithFullPath[]) {
	SDL_ShowCursor(1);
	freeResources();
	char command[200];
	char executable[200];
	strcpy(executable,determineExecutable(fileToBeExecutedWithFullPath));
	strcpy(command, executable);
	if(executable!=NULL) {
		strcat(command," ");
	}
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(command);
	if (returnValue==-1) {
		printf("ERROR");
	}
	SDL_Init(SDL_INIT_VIDEO);
	setupDisplay();
	setupDecorations();
}

void executeCommand (char executable[], char fileToBeExecutedWithFullPath[]) {
	SDL_ShowCursor(1);
	freeResources();
	char command[200];
	strcpy(command, executable);
	strcat(command," ");
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(command);
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

void loadGameList() {
	struct dirent **files;
	int n=scandir(menuSections[currentSection].filesDirectory, &files, 0, alphasort);
	int game = 0;
	int page = 0;
	for (int i=0;i<200;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<n;i++){
		if (strcmp((files[i]->d_name),".gitignore")!=0 && strcmp((files[i]->d_name),"..")!=0 && strcmp((files[i]->d_name),".")!=0){
			gameList[page][game] = files[i]->d_name;
			game++;
			if (game==ITEMS_PER_PAGE) {
				if(i!=n-1) {
					page++;
					totalPages++;
					game = 0;
				}
			}
		}
	}
	free(files);
}
