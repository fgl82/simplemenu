#include <constants.h>
#include <dirent.h>
#include <globals.h>
#include <screen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
#include <SDL/SDL.h>
#include <unistd.h>

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
		strcpy(aMenuSection.fileExtension,configurations[3]);
		printf("%s",aMenuSection.fileExtension);
		aMenuSection.headerAndFooterTextBackgroundColor.r=atoi(configurations[4]);
		aMenuSection.headerAndFooterTextBackgroundColor.g=atoi(configurations[5]);
		aMenuSection.headerAndFooterTextBackgroundColor.b=atoi(configurations[6]);
		aMenuSection.headerAndFooterTextForegroundColor.r=atoi(configurations[7]);
		aMenuSection.headerAndFooterTextForegroundColor.g=atoi(configurations[8]);
		aMenuSection.headerAndFooterTextForegroundColor.b=atoi(configurations[9]);
		aMenuSection.bodyBackgroundColor.r=atoi(configurations[10]);
		aMenuSection.bodyBackgroundColor.g=atoi(configurations[11]);
		aMenuSection.bodyBackgroundColor.b=atoi(configurations[12]);
		aMenuSection.bodyTextForegroundColor.r=atoi(configurations[13]);
		aMenuSection.bodyTextForegroundColor.g=atoi(configurations[14]);
		aMenuSection.bodyTextForegroundColor.b=atoi(configurations[15]);
		aMenuSection.bodySelectedTextBackgroundColor.r=atoi(configurations[16]);
		aMenuSection.bodySelectedTextBackgroundColor.g=atoi(configurations[17]);
		aMenuSection.bodySelectedTextBackgroundColor.b=atoi(configurations[18]);
		aMenuSection.bodySelectedTextForegroundColor.r=atoi(configurations[19]);
		aMenuSection.bodySelectedTextForegroundColor.g=atoi(configurations[20]);
		aMenuSection.bodySelectedTextForegroundColor.b=atoi(configurations[21]);
		menuSections[menuSectionCounter]=aMenuSection;
		menuSectionCounter++;
	}
	strcpy(menuSections[menuSectionCounter].sectionName,"END");
	fclose(fp);
	if (line) {
		free(line);
	}
}


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

int isExtensionValid(char *extension, struct MenuSection section) {
	printf("\n");
	printf("%s - %s\n", extension, section.fileExtension);
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
