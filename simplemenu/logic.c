#include <constants.h>
#include <config.h>
#include <dirent.h>
#include <globals.h>
#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
#include <unistd.h>

void quit() {
	freeResources();
	saveLastState();
	execlp("sh", "sh", "-c", "sync && poweroff", NULL);
}

int determineExecutable (char *fileToBeExecutedWithFullPath) {
	char *ext = getExtension(fileToBeExecutedWithFullPath);
	if (strcmp(ext,".gb")==0) {
		return 1;
	}
	if (strcmp(ext,".gbc")==0) {
		return 2;
	}
	if (strcmp(ext,".gba")==0) {
		return 3;
	}
	if (strcmp(ext,".nes")==0) {
		return 4;
	}
	if (strcmp(ext,".sfc")==0) {
		return 5;
	}
	if (strcmp(ext,".gg")==0) {
		return 6;
	}
	if (strcmp(ext,".sms")==0) {
		return 7;
	}
	if (strcmp(ext,".md")==0) {
		return 8;
	}
	if (strcmp(ext,".iso")==0) {
		return 9;
	}
	if (strcmp(ext,".zip")==0) {
		return 10;
	}
	if (strcmp(ext,".pce")==0) {
		return 11;
	}
	if (strcmp(ext,".bin")==0) {
		return 12;
	}
	return -1;
}

void executeFavorite (char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	int section = determineExecutable(fileToBeExecutedWithFullPath);
	strcpy(command, "cd ");
	strcat(command, menuSections[section].emulatorFolder);
	strcat(command, ";./");
	strcat(command, menuSections[section].executable);
	if(section!=-1) {
		strcat(command," ");
	}
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(strcat(command,"&>/dev/null"));
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

void executeCommand (char executable[], char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	strcpy(command, "cd ");
	strcat(command, menuSections[currentSection].emulatorFolder);
	strcat(command, ";./");
	strcat(command, executable);
	strcat(command," ");
	strcat(command,"\"");
	strcat(command,fileToBeExecutedWithFullPath);
	strcat(command,"\"");
	int returnValue = system(strcat(command,"&>/dev/null"));
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

int isExtensionValid(char *extension, struct MenuSection section) {
	if(currentSection>0) {
		return(strcmp(extension,section.fileExtension));
	}
	return 0;
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
		if (strcmp((files[i]->d_name),".gitignore")!=0 &&
				strcmp((files[i]->d_name),"..")!=0 &&
				strcmp((files[i]->d_name),".")!=0 &&
				isExtensionValid(getExtension((files[i]->d_name)),menuSections[currentSection])==0){
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
