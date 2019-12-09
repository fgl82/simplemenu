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
#include "../headers/ini2.h"

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
	char pathToScriptFiles[300];
	snprintf(pathToConfigFiles,sizeof(pathToConfigFiles),"%s/.simplemenu",getenv("HOME"));
	snprintf(pathToScriptFiles,sizeof(pathToConfigFiles),"%s/.simplemenu/scripts",getenv("HOME"));
	int directoryExists=mkdir(pathToConfigFiles,0700);
	if (!directoryExists) {
		char copyCommand[300];
		snprintf(copyCommand,sizeof(copyCommand),"cp ./config/* %s/.simplemenu/",getenv("HOME"));
		int ret = system(copyCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyScriptsCommand[300];
		mkdir(pathToScriptFiles,0700);
		snprintf(copyScriptsCommand,sizeof(copyScriptsCommand),"cp ./scripts/* %s/.simplemenu/scripts",getenv("HOME"));
		ret = system(copyScriptsCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
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
		} else if (i==4){
			OC_SLEEP=atoi(line);
		}  else if (i==5){
			stripGames=atoi(line);
		} else {
			shutDownEnabled=atoi(line);
		}
		i++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
}

uint32_t hex2int(char *hex) {
	char *hexCopy = malloc(strlen(hex)+1);
	strcpy(hexCopy,hex);
	uint32_t val = 0;
	while (*hexCopy) {
		// get current character then increment
		uint8_t byte = *hexCopy++;
		// transform hex character to the 4bit equivalent number, using the ascii table indexes
		if (byte >= '0' && byte <= '9') byte = byte - '0';
		else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
		else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
		// shift 4 to make space for new digit, and add the 4 bits of the new digit
		val = (val << 4) | (byte & 0xF);
	}
	return val;
}

int loadSections() {
	char pathToSectionsFilePlusFileName[300];
	snprintf(pathToSectionsFilePlusFileName,sizeof(pathToSectionsFilePlusFileName),"%s/.simplemenu/sections.cfg",getenv("HOME"));
	ini_t *config = ini_load(pathToSectionsFilePlusFileName);
	char *consoles = ini_get(config, "CONSOLES", "consoleList");
	char r[3];
	char g[3];
	char b[3];

	char*sectionNames[100];
	int sectionCounter=0;
	char* tokenizedSectionName=strtok(consoles,",");

	while(tokenizedSectionName!=NULL) {
		sectionNames[sectionCounter]=malloc(strlen(tokenizedSectionName)+1);
		strcpy(sectionNames[sectionCounter], tokenizedSectionName);
		tokenizedSectionName=strtok(NULL,",");
		sectionCounter++;
	}

	while(menuSectionCounter<sectionCounter) {
		char *sectionName = sectionNames[menuSectionCounter];
		strcpy(menuSections[menuSectionCounter].sectionName, sectionName);

		int dirCounter=0;
		for (int i=0;i<10;i++) {
			menuSections[menuSectionCounter].emulatorDirectories[dirCounter]=NULL;
		}

		char *value = ini_get(config, sectionName, "execDirs");
		char *value1 = malloc(3000);
		strcpy(value1,value);
		char *currentDir = strtok(value1,",");
		while(currentDir!=NULL) {
			menuSections[menuSectionCounter].emulatorDirectories[dirCounter]=malloc(strlen(currentDir)+1);
			strcpy(menuSections[menuSectionCounter].emulatorDirectories[dirCounter],currentDir);
			strcat(menuSections[menuSectionCounter].emulatorDirectories[dirCounter],"\0");
			currentDir = strtok(NULL,",");
			dirCounter++;
		}
		free (currentDir);
		menuSections[menuSectionCounter].emulatorDirectories[dirCounter]=NULL;
		menuSections[menuSectionCounter].activeEmulatorDirectory=0;

		int execCounter=0;
		value = ini_get(config, sectionName, "execs");
		char *value2 = malloc(3000);
		strcpy(value2,value);
		for (int i=0;i<10;i++) {
			menuSections[menuSectionCounter].executables[execCounter]=NULL;
		}

		char* currentExec = strtok(value2,",");
		while(currentExec!=NULL) {
			menuSections[menuSectionCounter].executables[execCounter]=malloc(strlen(currentExec)+1);
			strcpy(menuSections[menuSectionCounter].executables[execCounter],currentExec);
			currentExec = strtok(NULL,",");
			execCounter++;
		}
		free (currentExec);
		menuSections[menuSectionCounter].executables[execCounter]=NULL;
		menuSections[menuSectionCounter].activeExecutable=0;

		value = ini_get(config, sectionName, "romDirs");
		strcpy(menuSections[menuSectionCounter].filesDirectories,value);

		value = ini_get(config, sectionName, "romExts");
		strcpy(menuSections[menuSectionCounter].fileExtensions,value);

		value = ini_get(config, sectionName, "headerBackGround");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor.r=hex2int(r);
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor.g=hex2int(g);
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor.b=hex2int(b);

		value = ini_get(config, sectionName, "headerFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].headerAndFooterTextColor.r=hex2int(r);
		menuSections[menuSectionCounter].headerAndFooterTextColor.g=hex2int(g);
		menuSections[menuSectionCounter].headerAndFooterTextColor.b=hex2int(b);

		value = ini_get(config, sectionName, "bodyBackground");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodyBackgroundColor.r=hex2int(r);
		menuSections[menuSectionCounter].bodyBackgroundColor.g=hex2int(g);
		menuSections[menuSectionCounter].bodyBackgroundColor.b=hex2int(b);

		value = ini_get(config, sectionName, "bodyFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodyTextColor.r=hex2int(r);
		menuSections[menuSectionCounter].bodyTextColor.g=hex2int(g);
		menuSections[menuSectionCounter].bodyTextColor.b=hex2int(b);

		value = ini_get(config, sectionName, "selectedItemBackground");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.r=hex2int(r);
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.g=hex2int(g);
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.b=hex2int(b);

		value = ini_get(config, sectionName, "selectedItemFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodySelectedTextTextColor.r=hex2int(r);
		menuSections[menuSectionCounter].bodySelectedTextTextColor.g=hex2int(g);
		menuSections[menuSectionCounter].bodySelectedTextTextColor.b=hex2int(b);

		value = ini_get(config, sectionName, "logo");
		strcpy(menuSections[menuSectionCounter].consolePicture,value);

		value = ini_get(config, sectionName, "aliasFile");
		if(value!=NULL) {
			strcpy(menuSections[menuSectionCounter].aliasFileName,value);
		} else {
			strcpy(menuSections[menuSectionCounter].aliasFileName,"\0");
		}

		value = ini_get(config, sectionName, "category");
		strcpy(menuSections[menuSectionCounter].category,value);

		value = ini_get(config, sectionName, "onlyFileNamesNoPathOrExtension");
		if(strcmp("yes",value)==0) {
			menuSections[menuSectionCounter].onlyFileNamesNoExtension=1;
		}
		menuSections[menuSectionCounter].hidden=0;
		menuSections[menuSectionCounter].currentPage=0;
		menuSections[menuSectionCounter].currentGame=0;
		menuSectionCounter++;
	}
	strcpy(menuSections[menuSectionCounter].sectionName,"FAVORITES");
	menuSections[menuSectionCounter].emulatorDirectories[0]=malloc(strlen("/some/folder/")+1);
	strcpy(menuSections[menuSectionCounter].emulatorDirectories[0],"/some/folder/");
	strcat(menuSections[menuSectionCounter].emulatorDirectories[0],"\0");
	menuSections[menuSectionCounter].activeEmulatorDirectory=0;
	menuSections[menuSectionCounter].executables[0]=NULL;
	menuSections[menuSectionCounter].executables[0]=malloc(strlen("favs")+1);
	strcpy(menuSections[menuSectionCounter].executables[0],"favs");
	menuSections[menuSectionCounter].activeExecutable=0;
	strcpy(menuSections[menuSectionCounter].filesDirectories,"/some/folder");
	strcpy(menuSections[menuSectionCounter].fileExtensions,".zzz");
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor.r=27;
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor.g=31;
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor.b=34;
	menuSections[menuSectionCounter].headerAndFooterTextColor.r=231;
	menuSections[menuSectionCounter].headerAndFooterTextColor.g=109;
	menuSections[menuSectionCounter].headerAndFooterTextColor.b=62;
	menuSections[menuSectionCounter].bodyBackgroundColor.r=49;
	menuSections[menuSectionCounter].bodyBackgroundColor.g=58;
	menuSections[menuSectionCounter].bodyBackgroundColor.b=66;
	menuSections[menuSectionCounter].bodyTextColor.r=152;
	menuSections[menuSectionCounter].bodyTextColor.g=157;
	menuSections[menuSectionCounter].bodyTextColor.b=152;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.r=248;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.g=85;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor.b=41;
	menuSections[menuSectionCounter].bodySelectedTextTextColor.r=27;
	menuSections[menuSectionCounter].bodySelectedTextTextColor.g=33;
	menuSections[menuSectionCounter].bodySelectedTextTextColor.b=42;
	strcpy(menuSections[menuSectionCounter].category, "all");
	menuSections[menuSectionCounter].onlyFileNamesNoExtension=0;
	menuSections[menuSectionCounter].hidden=0;
	menuSections[menuSectionCounter].currentPage=0;
	menuSections[menuSectionCounter].currentGame=0;
	menuSectionCounter++;
	favoritesSectionNumber=menuSectionCounter-1;
	return menuSectionCounter;
}
