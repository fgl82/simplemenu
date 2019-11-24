#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/logic.h"
#include "../headers/string_utils.h"

void loadAliasList(int sectionNumber) {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE *aliasFile=getCurrentSectionAliasFile();
	if (aliasFile==NULL) {
		strcpy(menuSections[sectionNumber].aliasFileName," ");
		return;
	}
	menuSections[sectionNumber].aliasHashTable = ht_create(10000);
	while ((read = getline(&line, &len, aliasFile)) != -1) {
		char *romName = strtok(line, "=");
		char *alias = strtok(NULL, "=");
		int lastChar = strlen(alias)-1;
		alias[lastChar]='\0';
		ht_set(menuSections[sectionNumber].aliasHashTable, romName, alias);
	}
	if (line) {
		free(line);
	}
	fclose(aliasFile);
}

void createConfigFilesInHomeIfTheyDontExist() {
	char pathToConfigFiles[300];
	snprintf(pathToConfigFiles,sizeof(pathToConfigFiles),"%s/.simplemenu",getenv("HOME"));
	int directoryExists=mkdir(pathToConfigFiles,0700);
	if (!directoryExists) {
		char copyCommand[300];
		snprintf(copyCommand,sizeof(copyCommand),"cp ./config/* %s/.simplemenu/",getenv("HOME"));
		system(copyCommand);
	}
}

void saveFavorites() {
	if (favoritesChanged) {
		FILE * fp;
		char pathToFavoritesFilePlusFileName[300];
		snprintf(pathToFavoritesFilePlusFileName,sizeof(pathToFavoritesFilePlusFileName),"%s/.simplemenu/favorites.sav",getenv("HOME"));
		fp = fopen(pathToFavoritesFilePlusFileName, "w");
		int linesWritten=0;
		for (int j=0;j<favoritesSize;j++) {
			struct Favorite favorite = favorites[j];
			if (strlen(favorite.name)==1) {
				break;
			}
			if(linesWritten>0) {
				fprintf(fp,"\n");
			}
//			favorite=findFavorite(favorite.alias);
			fprintf(fp,"%s;",favorite.section);
			fprintf(fp,"%s;",favorite.name);
			if(favorite.alias[0]=='\0') {
				fprintf(fp," ;");
			} else {
				fprintf(fp,"%s;",favorite.alias);
			}
			fprintf(fp,"%s;",favorite.emulatorFolder);
			fprintf(fp,"%s;",favorite.executable);
			fprintf(fp,"%s",favorite.filesDirectory);
			linesWritten++;
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
	char pathToFavoritesFilePlusFileName[300];
	snprintf(pathToFavoritesFilePlusFileName,sizeof(pathToFavoritesFilePlusFileName),"%s/.simplemenu/favorites.sav",getenv("HOME"));
	fp = fopen(pathToFavoritesFilePlusFileName, "r");
	if (fp==NULL) {
		generateError("FAVORITES FILE NOT FOUND-SHUTTING DOWN",1);
		return;
	}
	char *configurations[6];
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
		strcpy(favorites[favoritesSize].section,configurations[0]);
		strcpy(favorites[favoritesSize].name,configurations[1]);
		strcpy(favorites[favoritesSize].alias,configurations[2]);
		strcpy(favorites[favoritesSize].emulatorFolder,configurations[3]);
		strcpy(favorites[favoritesSize].executable,configurations[4]);
		strcpy(favorites[favoritesSize].filesDirectory,configurations[5]);
		int len = strlen(favorites[favoritesSize].filesDirectory);
		if (favorites[favoritesSize].filesDirectory[len-1]=='\n') {
			favorites[favoritesSize].filesDirectory[len-1]='\0';
		}
		favoritesSize++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
	qsort(favorites, favoritesSize, sizeof(struct Favorite), compareFavorites);
}

void saveLastState() {
	FILE * fp;
	char pathToStatesFilePlusFileName[300];
	snprintf(pathToStatesFilePlusFileName,sizeof(pathToStatesFilePlusFileName),"%s/.simplemenu/last_state.cfg",getenv("HOME"));
	fp = fopen(pathToStatesFilePlusFileName, "w");
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
	char pathToStatesFilePlusFileName[300];
	snprintf(pathToStatesFilePlusFileName,sizeof(pathToStatesFilePlusFileName),"%s/.simplemenu/last_state.cfg",getenv("HOME"));
	fp = fopen(pathToStatesFilePlusFileName, "r");
	if (fp==NULL) {
		generateError("STATE FILE NOT FOUND-SHUTTING DOWN",1);
		return;
	}
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
			if(strlen(CURRENT_SECTION.sectionName)<1) {
				continue;
			}			
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

void loadConfig() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char pathToConfigFilePlusFileName[300];
	snprintf(pathToConfigFilePlusFileName,sizeof(pathToConfigFilePlusFileName),"%s/.simplemenu/config.cfg",getenv("HOME"));
	fp = fopen(pathToConfigFilePlusFileName, "r");
	int i=0;
	while ((read = getline(&line, &len, fp)) != -1) {
		if(line[0]=='#') {
			continue;
		}
		if (i==0) {
			OC_UC=atoi(line);
		} else if (i==1) {
			OC_NO=atoi(line);
		} else if (i==2){
			OC_OC=atoi(line);
		} else if (i==3){
			timeoutValue=atoi(line);
		} else {
			OC_SLEEP=atoi(line);
		}
		i++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
}

int loadSections() {
	FILE * fp;
	char line[1500];
	char *configurations[25];
	char pathToSectionsFilePlusFileName[300];
	snprintf(pathToSectionsFilePlusFileName,sizeof(pathToSectionsFilePlusFileName),"%s/.simplemenu/sections.cfg",getenv("HOME"));
	fp = fopen(pathToSectionsFilePlusFileName, "r");
	if (fp==NULL) {
		return -1;
	}
	while (fgets(line, sizeof(line), fp) != NULL) {
		if(line[0]=='#'||strlen(line)<2) {
			continue;
		}	
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
		strcpy(aMenuSection.filesDirectories,configurations[3]);
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
		strcpy(aMenuSection.aliasFileName,configurations[24]);
		if (strlen(aMenuSection.aliasFileName)>1) {
			aMenuSection.aliasFileName[strlen(aMenuSection.aliasFileName)-1]='\0';
		}
		aMenuSection.hidden=0;
		aMenuSection.currentPage=0;
		aMenuSection.currentGame=0;
		menuSections[menuSectionCounter]=aMenuSection;
		menuSections[menuSectionCounter].hidden=0;
		menuSectionCounter++;
	}
	strcpy(line,"FAVORITES;/some/folder/;favs;/some/folder/;.zzz;27;31;34;231;109;62;49;58;66;152;157;152;248;85;41;27;33;42;none;");
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
	strcpy(aMenuSection.filesDirectories,configurations[3]);
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
	strcpy(aMenuSection.aliasFileName,configurations[24]);
	if (strlen(aMenuSection.aliasFileName)>1) {
		aMenuSection.aliasFileName[strlen(aMenuSection.aliasFileName)-1]='\0';
	}
	aMenuSection.hidden=0;
	aMenuSection.currentPage=0;
	aMenuSection.currentGame=0;
	menuSections[menuSectionCounter]=aMenuSection;
	menuSections[menuSectionCounter].hidden=0;
	menuSectionCounter++;
	favoritesSectionNumber=menuSectionCounter-1;
	fclose(fp);
	return menuSectionCounter;
}
