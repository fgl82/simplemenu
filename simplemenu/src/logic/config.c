#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/logic.h"

void saveFavorites() {
	if (favoritesChanged) {
		FILE * fp;
		fp = fopen("./config/favorites.sav", "w");
		struct Favorite favorite;
		char *name;
		for (int j=0;j<1000;j++) {
			for (int k=0;k<10;k++) {
				name = FAVORITES_SECTION.gameList[j][k];
				if (name!=NULL) {
					favorite=findFavorite(name);
					fprintf(fp,"%s;",favorite.name);
					fprintf(fp,"%s;",favorite.emulatorFolder);
					fprintf(fp,"%s;",favorite.executable);
					fprintf(fp,"%s\n",favorite.filesDirectory);
				}
			}
			if (name==NULL) {
				break;
			}
		}
		fclose(fp);
		favoritesChanged=0;
	}
}

void loadFavorites() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("./config/favorites.sav", "r");
	char *configurations[4];
	char *ptr;
	favoritesSize=0;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		strcpy(favorites[favoritesSize].name,configurations[0]);
		strcpy(favorites[favoritesSize].emulatorFolder,configurations[1]);
		strcpy(favorites[favoritesSize].executable,configurations[2]);
		strcpy(favorites[favoritesSize].filesDirectory,configurations[3]);
		int len = strlen(favorites[favoritesSize].filesDirectory);
		favorites[favoritesSize].filesDirectory[len-1]='\0';
		favoritesSize++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
}

void saveLastState() {
	FILE * fp;
	fp = fopen("./config/last_state.cfg", "w");
	fprintf(fp, "%d;\n", currentSectionNumber);
	for (currentSectionNumber=0;currentSectionNumber<menuSectionCounter;currentSectionNumber++) {
		fprintf(fp, "%d;%d;%d\n", currentSectionNumber, CURRENT_SECTION.currentPage, CURRENT_SECTION.currentGame);
	}
	fclose(fp);
}

void loadLastState() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("./config/last_state.cfg", "r");
	char *configurations[4];
	char *ptr;
	int first = -1;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		if (first==-1) {
			first=atoi(configurations[0]);
		} else {
			currentSectionNumber=atoi(configurations[0]);
			CURRENT_SECTION.currentPage=atoi(configurations[1]);
			CURRENT_SECTION.currentGame=atoi(configurations[2]);
			CURRENT_SECTION.alphabeticalPaging=0;
		}
	}
	currentSectionNumber=first;
	fclose(fp);
	if (line) {
		free(line);
	}
}

int loadConfig() {
	FILE * fp;
	char line[500];
	char *configurations[24];
	fp = fopen("./config/sections.cfg", "r");
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		int i=0;
		char *ptr = strtok(line, ";");
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		struct MenuSection aMenuSection;
		strcpy(aMenuSection.sectionName,configurations[0]);
		strcpy(aMenuSection.emulatorFolder,configurations[1]);
		strcpy(aMenuSection.executable,configurations[2]);
		strcpy(aMenuSection.filesDirectory,configurations[3]);
		strcpy(aMenuSection.fileExtensions,configurations[4]);
		aMenuSection.headerAndFooterTextBackgroundColor.r=atoi(configurations[5]);
		aMenuSection.headerAndFooterTextBackgroundColor.g=atoi(configurations[6]);
		aMenuSection.headerAndFooterTextBackgroundColor.b=atoi(configurations[7]);
		aMenuSection.headerAndFooterTextForegroundColor.r=atoi(configurations[8]);
		aMenuSection.headerAndFooterTextForegroundColor.g=atoi(configurations[9]);
		aMenuSection.headerAndFooterTextForegroundColor.b=atoi(configurations[10]);
		aMenuSection.bodyBackgroundColor.r=atoi(configurations[11]);
		aMenuSection.bodyBackgroundColor.g=atoi(configurations[12]);
		aMenuSection.bodyBackgroundColor.b=atoi(configurations[13]);
		aMenuSection.bodyTextForegroundColor.r=atoi(configurations[14]);
		aMenuSection.bodyTextForegroundColor.g=atoi(configurations[15]);
		aMenuSection.bodyTextForegroundColor.b=atoi(configurations[16]);
		aMenuSection.bodySelectedTextBackgroundColor.r=atoi(configurations[17]);
		aMenuSection.bodySelectedTextBackgroundColor.g=atoi(configurations[18]);
		aMenuSection.bodySelectedTextBackgroundColor.b=atoi(configurations[19]);
		aMenuSection.bodySelectedTextForegroundColor.r=atoi(configurations[20]);
		aMenuSection.bodySelectedTextForegroundColor.g=atoi(configurations[21]);
		aMenuSection.bodySelectedTextForegroundColor.b=atoi(configurations[22]);
		strcpy(aMenuSection.consolePicture,configurations[23]);
		aMenuSection.consolePicture[strlen(aMenuSection.consolePicture)-1]='\0';
		aMenuSection.hidden=0;
		aMenuSection.currentPage=0;
		aMenuSection.currentGame=0;
		menuSections[menuSectionCounter]=aMenuSection;
		menuSections[menuSectionCounter].hidden=0;
		menuSectionCounter++;
	}
	favoritesSectionNumber=menuSectionCounter-1;
	fclose(fp);
	return menuSectionCounter;
}
