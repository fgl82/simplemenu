#include <definitions.h>
#include <globals.h>
#include <logic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void saveFavorites() {
	if (favoritesChanged) {
		FILE * fp;
		fp = fopen("./config/favorites.sav", "w");
		for (int i=0;i<favoritesSize;i++) {
			fprintf(fp,"%s;",favorites[i].name);
			fprintf(fp,"%s;",favorites[i].emulatorFolder);
			fprintf(fp,"%s;",favorites[i].executable);
			fprintf(fp,"%s\n",favorites[i].filesDirectory);
		}
		fclose(fp);
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
	fprintf(fp, "%d;%d;%d", currentSectionNumber, CURRENT_SECTION.currentPage, CURRENT_SECTION.currentGame);
	fclose(fp);
}

void loadLastState() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	fp = fopen("./config/last_state.cfg", "r");
	char *configurations[3];
	char *ptr;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		currentSectionNumber=atoi(configurations[0]);
		CURRENT_SECTION.currentPage=atoi(configurations[1]);
		CURRENT_SECTION.currentGame=atoi(configurations[2]);
	}
	fclose(fp);
	if (line) {
		free(line);
	}
}

void loadConfig() {
	FILE * fp;
	char line[500];
	char *configurations[23];
	fp = fopen("./config/sections.cfg", "r");
	int menuSectionCounter = 0;
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		int i=0;
		char *ptr = strtok(line, ";");
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		if (strcmp(configurations[2],"favs")==0||countFiles(configurations[3])>2) {
			struct MenuSection aMenuSection;
			strcpy(aMenuSection.sectionName,configurations[0]);
			strcpy(aMenuSection.emulatorFolder,configurations[1]);
			strcpy(aMenuSection.executable,configurations[2]);
			strcpy(aMenuSection.filesDirectory,configurations[3]);
			strcpy(aMenuSection.fileExtension,configurations[4]);
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
			aMenuSection.currentPage=0;
			aMenuSection.currentGame=0;
			menuSections[menuSectionCounter]=aMenuSection;
			menuSectionCounter++;
		}
	}
	lastSection=menuSectionCounter-1;
	fclose(fp);
}
