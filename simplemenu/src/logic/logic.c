#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/config.h"
#include "../headers/constants.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"

void quit() {
	freeResources();
	saveLastState();
	saveFavorites();
	execlp("sh", "sh", "-c", "sync && poweroff", NULL);
}

struct Favorite findFavorite(char *name) {
	struct Favorite favorite;
	for (int i=0; i<favoritesSize;i++) {
		favorite = favorites[i];
		if (strcmp(favorite.name, name)==0) {
			return favorite;
		}
	}
	return favorite;
}

int doesFavoriteExist(char *name) {
	for(int i=0;i<favoritesSize;i++) {
		if (strcmp(favorites[i].name,name)==0) {
			return 1;
		}
	}
	return 0;
}

void setSectionsState(char *states) {
	char *endSemiColonStr;
	char *semiColonToken = strtok_r(states, ";", &endSemiColonStr);
	int i=0;
	while (semiColonToken != NULL)
	{
		char *endDashToken;
		char *dashToken = strtok_r(semiColonToken, "-", &endDashToken);
		int j=0;
		while (dashToken != NULL)
		{
			if (j==0) {
				menuSections[i].currentPage=atoi(dashToken);
			} else if (j==1) {
				menuSections[i].currentGame=atoi(dashToken);
			} else {
				menuSections[i].alphabeticalPaging=atoi(dashToken);
			}
			j++;
			dashToken = strtok_r(NULL, "-", &endDashToken);
		}
		semiColonToken = strtok_r(NULL, ";", &endSemiColonStr);
		i++;
	}
}

void executeCommand (char *emulatorFolder, char *executable, char *fileToBeExecutedWithFullPath) {
	char states[200]="";
	for (int i=0;i<favoritesSectionNumber+1;i++) {
		char tempString[200]="";
		snprintf(tempString,sizeof(tempString),"%d-%d-%d;",menuSections[i].currentPage,menuSections[i].currentGame,menuSections[i].alphabeticalPaging);
		strcat(states,tempString);
	}
	char pReturnTo[3];
	snprintf(pReturnTo,sizeof(pReturnTo),"%d;",returnTo);
	char pSectionNumber[3]="";
	char pPictureMode[2]="";
	snprintf(pSectionNumber,sizeof(pSectionNumber),"%d",currentSectionNumber);
	snprintf(pPictureMode,sizeof(pPictureMode),"%d",pictureMode);
	execlp("./invoker.elf","invoker.elf", emulatorFolder, executable, fileToBeExecutedWithFullPath, states, pSectionNumber, pReturnTo, pPictureMode, NULL);
}

int isExtensionValid(char *extension, char *fileExtensions) {
	char fileExtensionsCopy[200];
	strcpy(fileExtensionsCopy, fileExtensions);
	char *ptr = strtok(fileExtensionsCopy, ",");
	int i=0;
	while(ptr != NULL) {
		int areStringsDifferent = strcmp(extension,ptr);
		if (!areStringsDifferent) {
			return(1);
		}
		ptr = strtok(NULL, ",");
		i++;
	}
	return(0);
}

void swap_str_ptrs(char **arg1, char **arg2)
{
	char *tmp = *arg1;
	*arg1 = *arg2;
	*arg2 = tmp;
}

void sortGames(char *names[], unsigned int len)
{
	unsigned int i, pvt=0;
	if (len <= 1)
		return;
	swap_str_ptrs(names+((unsigned int)rand() % len), names+len-1);
	for (i=0;i<len-1;++i)
	{
		char *first = toLower(names[i]);
		char *second = toLower(names[len-1]);
		if (strcmp(first, second) < 0)
			swap_str_ptrs(names+i, names+pvt++);
		free(first);
		free(second);
	}
	swap_str_ptrs(names+pvt, names+len-1);
	sortGames(names, pvt++);
	sortGames(names+pvt, len - pvt);
}

int countGamesInSection() {
	int gamesCounter=0;
	for (int i=0;i<=CURRENT_SECTION.totalPages;i++) {
		for (int j=0;j<ITEMS_PER_PAGE;j++) {
			if (CURRENT_SECTION.gameList[i][j]!=NULL) {
				gamesCounter++;
			}
		}
	}
	return gamesCounter;
}

void loadFavoritesSectionGameList() {
	int game = 0;
	int page = 0;
	FAVORITES_SECTION.totalPages=0;
	for (int i=0;i<1000;i++) {
		for (int j=0;j<10;j++) {
			FAVORITES_SECTION.gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<favoritesSize;i++){
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize) {
				page++;
				game = 0;
				FAVORITES_SECTION.totalPages++;
			}
		}
		FAVORITES_SECTION.gameList[page][game] = favorites[i].name;
		game++;
	}
	char ** pepe =*FAVORITES_SECTION.gameList;
	sortGames(pepe,countGamesInSection());
}

int recursivelyScanDirectory (char *directory, char* files[])
{
	DIR * d;
	d = opendir (directory);
	int i=0;
	while (1) {
		struct dirent *entry;
		const char * d_name;
		entry = readdir (d);
		if (!entry) {
			break;
		}
		d_name = entry->d_name;
		if (entry->d_type & DT_DIR) {
			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				char path[PATH_MAX];
				snprintf (path, PATH_MAX, "%s/%s", directory, d_name);
				i+=recursivelyScanDirectory(path, files);
			}
		} else {
			files[i]=malloc(sizeof(entry->d_name));
			strcpy(files[i],entry->d_name);
			i++;
		}
	}
	return i;
}

void loadGameList() {
	int loadedFiles=0;
	printf("NOT YET FOR SECTION %d\n", currentSectionNumber);
	if (CURRENT_SECTION.gameList[0][0] == NULL) {
		printf("WENT IN FOR SECTION %d\n", currentSectionNumber);
		CURRENT_SECTION.totalPages=0;
		char *files[8000];
		int n = recursivelyScanDirectory(CURRENT_SECTION.filesDirectory, files);
		int game = 0;
		int page = 0;
		for (int i=0;i<n;i++){
			char path[3000] = "";
			strcpy(path,CURRENT_SECTION.filesDirectory);
			strcat(path,files[i]);
			char *ext = getExtension(files[i]);
			if (ext&&strcmp((files[i]),"..")!=0 &&
					strcmp((files[i]),".")!=0 &&
					strcmp(ext,".png")!=0&&
					isExtensionValid(ext,CURRENT_SECTION.fileExtensions)){
				int size = strlen(files[i])+1;
				CURRENT_SECTION.gameList[page][game]=malloc(size);
				if (game==ITEMS_PER_PAGE) {
					if(i!=n) {
						page++;
						CURRENT_SECTION.totalPages++;
						game = 0;
					}
				}
				strcpy(CURRENT_SECTION.gameList[page][game],files[i]);
				loadedFiles++;
				strcat(CURRENT_SECTION.gameList[page][game],"\0");
				game++;
			}
		}
		if (loadedFiles==0) {
			printf("0 pages %d\n", currentSectionNumber);
			CURRENT_SECTION.hidden=1;
		}
		for (int i=0;i<n;i++){
			free(files[i]);
		}
//		free(files);
		char ** pepe =*CURRENT_SECTION.gameList;
		sortGames(pepe,countGamesInSection());
	}
}

int countGamesInPage() {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesCounter++;
		}
	}
	return gamesCounter;
}

int getFirstNonHiddenSection(int sectionCount) {
	for (int i=0;i<sectionCount;i++) {
		if (menuSections[i].hidden==0) {
			return i;
		}
	}
	return 0;
}

void determineStartingScreen(int sectionCount) {
	if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
		favoritesSectionSelected=1;
		loadFavoritesSectionGameList();
	} else {
		loadGameList();
		if(CURRENT_SECTION.hidden) {
			int advanced = advanceSection();
			loadGameList();
			if(advanced) {
				while(CURRENT_SECTION.hidden) {
					advanceSection();
					loadGameList();
				}
			}
		}
	}
}
