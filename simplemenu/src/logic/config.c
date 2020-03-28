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
	menuSections[sectionNumber].aliasHashTable = ht_create(MAX_GAMES_IN_SECTION);
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
	char pathToAppFiles[300];
	char pathToGameFiles[300];
	snprintf(pathToConfigFiles,sizeof(pathToConfigFiles),"%s/.simplemenu",getenv("HOME"));
	snprintf(pathToAppFiles,sizeof(pathToConfigFiles),"%s/.simplemenu/apps",getenv("HOME"));
	snprintf(pathToGameFiles,sizeof(pathToGameFiles),"%s/.simplemenu/games",getenv("HOME"));
	int directoryExists=mkdir(pathToConfigFiles,0700);
	if (!directoryExists) {
		char copyCommand[300];
		snprintf(copyCommand,sizeof(copyCommand),"cp ./config/* %s/.simplemenu/",getenv("HOME"));
		int ret = system(copyCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyAppsCommand[300];
		mkdir(pathToAppFiles,0700);
		snprintf(copyAppsCommand,sizeof(copyAppsCommand),"cp ./apps/* %s/.simplemenu/apps",getenv("HOME"));
		ret = system(copyAppsCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyGamesCommand[300];
		mkdir(pathToGameFiles,0700);
		snprintf(copyGamesCommand,sizeof(copyGamesCommand),"cp ./games/* %s/.simplemenu/games",getenv("HOME"));
		ret = system(copyGamesCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char deleteDirectoriesCommand[300];
		snprintf(deleteDirectoriesCommand,sizeof(deleteDirectoriesCommand),"rm -rf ./apps");
		ret = system(deleteDirectoriesCommand);
		snprintf(deleteDirectoriesCommand,sizeof(deleteDirectoriesCommand),"rm -rf ./games");
		ret = system(deleteDirectoriesCommand);
		snprintf(deleteDirectoriesCommand,sizeof(deleteDirectoriesCommand),"rm -rf ./config");
		ret = system(deleteDirectoriesCommand);
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
	fprintf(fp, "%d;\n", stripGames);
	fprintf(fp, "%d;\n", fullscreenMode);
	fprintf(fp, "%d;\n", currentSectionNumber);
	for (currentSectionNumber=0;currentSectionNumber<menuSectionCounter;currentSectionNumber++) {
		fprintf(fp, "%d;%d;%d;%d\n", currentSectionNumber, CURRENT_SECTION.currentPage, CURRENT_SECTION.currentGameInPage, CURRENT_SECTION.realCurrentGameNumber);
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
	char *configurations[5];
	char *ptr;
	int startInSection = -1;
	int startInPictureMode = -1;
	int stripGamesConfig = -1;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		if (stripGamesConfig==-1) {
			stripGamesConfig=atoi(configurations[0]);
		} else if (startInPictureMode==-1){
			startInPictureMode=atoi(configurations[0]);
		} else if (startInSection==-1) {
			startInSection=atoi(configurations[0]);
		} else {
			currentSectionNumber=atoi(configurations[0]);
			if(strlen(CURRENT_SECTION.sectionName)<1) {
				continue;
			}
			int page = atoi(configurations[1]);
			int game = atoi(configurations[2]);
			int realCurrentGameNumber = atoi(configurations[3]);
			CURRENT_SECTION.currentPage = page;
			CURRENT_SECTION.currentGameInPage=game;
			CURRENT_SECTION.realCurrentGameNumber=realCurrentGameNumber;
			CURRENT_SECTION.alphabeticalPaging=0;
		}
	}
	stripGames=stripGamesConfig;
	currentSectionNumber=startInSection;
	fullscreenMode=startInPictureMode;
	fclose(fp);
	if (line) {
		free(line);
	}
}

void loadConfig() {
	char pathToConfigFilePlusFileName[3000];
	snprintf(pathToConfigFilePlusFileName,sizeof(pathToConfigFilePlusFileName),"%s/.simplemenu/config.cfg",getenv("HOME"));
	ini_t *config = ini_load(pathToConfigFilePlusFileName);

	const char *value = ini_get(config, "CPU", "underclocked_speed");
	OC_UC=atoi(value);

	value = ini_get(config, "CPU", "normal_speed");
	OC_NO=atoi(value);

	value = ini_get(config, "CPU", "overclocked_speed");
	OC_NO=atoi(value);

	value = ini_get(config, "CPU", "sleep_speed");
	OC_SLEEP=atoi(value);

	value = ini_get(config, "SYSTEM", "screen_timeout_in_seconds");
	timeoutValue=atoi(value);

	value = ini_get(config, "SYSTEM", "allow_shutdown");
	shutDownEnabled=atoi(value);

	value = ini_get(config, "MENU_MODE", "logo_background");
	strcpy(simpleBackground,value);

	value = ini_get(config, "FULLSCREEN_MODE", "display_footer");
	footerVisibleInFullscreenMode=atoi(value);

	value = ini_get(config, "FULLSCREEN_MODE", "display_menu");
	menuVisibleInFullscreenMode=atoi(value);

	value = ini_get(config, "FULLSCREEN_MODE", "background");
	strcpy(fullscreenBackground,value);

	value = ini_get(config, "GENERAL", "font");
	strcpy(menuFont,value);

	value = ini_get(config, "GENERAL", "media_folder");
	strcpy(mediaFolder,value);

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
	const char *consoles = ini_get(config, "CONSOLES", "consoleList");
	char r[3];
	char g[3];
	char b[3];
	char *consoles1 = strdup(consoles);
	char *sectionNames[10000];
	int sectionCounter=0;

	char* tokenizedSectionName=strtok(consoles1,",");
	while(tokenizedSectionName!=NULL) {
		sectionNames[sectionCounter]=malloc(strlen(tokenizedSectionName)+1);
		strcpy(sectionNames[sectionCounter], tokenizedSectionName);
		tokenizedSectionName=strtok(NULL,",");
		sectionCounter++;
	}

	while(menuSectionCounter<sectionCounter) {
		char *sectionName = sectionNames[menuSectionCounter];
		strcpy(menuSections[menuSectionCounter].sectionName, sectionName);

		//READ DIR
		int dirCounter=0;
		const char *value = ini_get(config, sectionName, "execDirs");
		for (int i=0;i<10;i++) {
			menuSections[menuSectionCounter].emulatorDirectories[i]=NULL;
		}
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

		//READ EXEC
		int execCounter=0;
		value = ini_get(config, sectionName, "execs");
		char *value2 = malloc(3000);
		strcpy(value2,value);
		for (int i=0;i<10;i++) {
			menuSections[menuSectionCounter].executables[i]=NULL;
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
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor[0]=hex2int(r);
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor[1]=hex2int(g);
		menuSections[menuSectionCounter].headerAndFooterBackgroundColor[2]=hex2int(b);

		value = ini_get(config, sectionName, "headerFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].headerAndFooterTextColor[0]=hex2int(r);
		menuSections[menuSectionCounter].headerAndFooterTextColor[1]=hex2int(g);
		menuSections[menuSectionCounter].headerAndFooterTextColor[2]=hex2int(b);

		value = ini_get(config, sectionName, "bodyBackground");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodyBackgroundColor[0]=hex2int(r);
		menuSections[menuSectionCounter].bodyBackgroundColor[1]=hex2int(g);
		menuSections[menuSectionCounter].bodyBackgroundColor[2]=hex2int(b);

		value = ini_get(config, sectionName, "bodyFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodyTextColor[0]=hex2int(r);
		menuSections[menuSectionCounter].bodyTextColor[1]=hex2int(g);
		menuSections[menuSectionCounter].bodyTextColor[2]=hex2int(b);

		value = ini_get(config, sectionName, "selectedItemBackground");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[0]=hex2int(r);
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[1]=hex2int(g);
		menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[2]=hex2int(b);

		value = ini_get(config, sectionName, "selectedItemFont");
		r[0]=value[0];r[1]=value[1];r[2]='\0';g[0]=value[2];g[1]=value[3];g[2]='\0';b[0]=value[4];b[1]=value[5];b[2]='\0';
		menuSections[menuSectionCounter].bodySelectedTextTextColor[0]=hex2int(r);
		menuSections[menuSectionCounter].bodySelectedTextTextColor[1]=hex2int(g);
		menuSections[menuSectionCounter].bodySelectedTextTextColor[2]=hex2int(b);

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
//		theSectionHasGames(&menuSections[menuSectionCounter]);
		menuSections[menuSectionCounter].currentPage=0;
		menuSections[menuSectionCounter].currentGameInPage=0;
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
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor[0]=27;
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor[1]=31;
	menuSections[menuSectionCounter].headerAndFooterBackgroundColor[2]=34;
	menuSections[menuSectionCounter].headerAndFooterTextColor[0]=231;
	menuSections[menuSectionCounter].headerAndFooterTextColor[1]=109;
	menuSections[menuSectionCounter].headerAndFooterTextColor[2]=62;
	menuSections[menuSectionCounter].bodyBackgroundColor[0]=49;
	menuSections[menuSectionCounter].bodyBackgroundColor[1]=58;
	menuSections[menuSectionCounter].bodyBackgroundColor[2]=66;
	menuSections[menuSectionCounter].bodyTextColor[0]=152;
	menuSections[menuSectionCounter].bodyTextColor[1]=157;
	menuSections[menuSectionCounter].bodyTextColor[2]=152;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[0]=248;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[1]=85;
	menuSections[menuSectionCounter].bodySelectedTextBackgroundColor[2]=41;
	menuSections[menuSectionCounter].bodySelectedTextTextColor[0]=27;
	menuSections[menuSectionCounter].bodySelectedTextTextColor[1]=33;
	menuSections[menuSectionCounter].bodySelectedTextTextColor[2]=42;
	strcpy(menuSections[menuSectionCounter].category, "all");
	menuSections[menuSectionCounter].onlyFileNamesNoExtension=0;
	menuSections[menuSectionCounter].hidden=0;
	menuSections[menuSectionCounter].currentPage=0;
	menuSections[menuSectionCounter].currentGameInPage=0;
	strcpy(menuSections[menuSectionCounter].consolePicture,"./resources/favorites/logo.png");
	menuSectionCounter++;
	favoritesSectionNumber=menuSectionCounter-1;
	return menuSectionCounter;
}
