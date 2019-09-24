#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/config.h"
#include "../headers/constants.h"
#include "../headers/control.h"
#include "../headers/globals.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/definitions.h"

void quit() {
	freeResources();
	saveLastState();
	saveFavorites();
	execlp("sh", "sh", "-c", "sync && poweroff", NULL);
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

//int is_regular_file(const char *path)
//{
//	struct stat path_stat;
//	stat(path, &path_stat);
//	return S_ISREG(path_stat.st_mode);
//}

int countFiles (char* directoryName, char *fileExtensions) {
	struct dirent **files;
	int filescount = scandir(directoryName, &files, 0, alphasort);
	int result=0;
	for (int i=0;i<filescount;i++){
		char path[2000] = "";
		char *ext = getExtension(files[i]->d_name);
		if (ext&&strcmp((files[i]->d_name),"..")!=0 && strcmp((files[i]->d_name),".")!=0&&strcmp(ext,".png")!=0) {
			strcpy(path,directoryName);
			strcat(path,files[i]->d_name);
			if(isExtensionValid(ext,fileExtensions)) {
				result++;
			}
		}
	}
	return result;
}

void sortFavorites() {
	struct Favorite tmp;
	for(int i=0; i<favoritesSize; i++) {
		for(int j = 0; j<favoritesSize; j++) {
			char *first = toLower(favorites[i].name);
			char *second = toLower(favorites[j].name);
			if(strcmp(first, second) < 0) {
				tmp = favorites[i];
				favorites[i] = favorites[j];
				favorites[j] = tmp;
			}
			free(first);
			free(second);
		}
	}
}

void sortGameList(struct dirent* files[], int length) {
	struct dirent *tmp;
	for(int i=0; i<length; i++) {
		for(int j = 0; j<length; j++) {
			char *first = toLower(files[i]->d_name);
			char *second = toLower(files[j]->d_name);
			if(strcmp(first, second) < 0) {
				tmp = files[i];
				files[i] = files[j];
				files[j] = tmp;
			}
			free(first);
			free(second);
		}
	}
}


void loadFavoritesList() {
	int game = 0;
	int page = 0;
	totalPages=0;
	for (int i=0;i<1000;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<favoritesSize;i++){
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize-1) {
				page++;
				totalPages++;
				game = 0;
			}
		}
		gameList[page][game] = favorites[i].name;
		game++;
	}
	sortFavorites();
}

void loadGameList() {
	totalPages=0;
	struct dirent **files;
	int n=scandir(CURRENT_SECTION.filesDirectory, &files, 0, alphasort);
	int game = 0;
	int page = 0;
	for (int i=0;i<1000;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	int lastRound=0;
//	sortGameList(files, n);
	for (int i=0;i<n;i++){
		char path[2000] = "";
		strcpy(path,CURRENT_SECTION.filesDirectory);
		strcat(path,files[i]->d_name);
		char *ext = getExtension(files[i]->d_name);
		if (ext&&strcmp((files[i]->d_name),"..")!=0 &&
				strcmp((files[i]->d_name),".")!=0 &&
				strcmp(ext,".png")!=0&&
				isExtensionValid(ext,CURRENT_SECTION.fileExtensions)){
			lastRound=0;
			if (game==ITEMS_PER_PAGE) {
				if(i!=n-1) {
					page++;
					totalPages++;
					game = 0;
					lastRound=1;
				}
			}
			gameList[page][game] = files[i]->d_name;
			game++;
		}
	}
	if (lastRound==1) {
		totalPages--;
	}
	if (totalPages<0) {
		CURRENT_SECTION.hidden=1;
	}
	free(files);
}

int countGamesInPage() {
	int gamesCounter=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (gameList[menuSections[currentSectionNumber].currentPage][i]!=NULL) {
			gamesCounter++;
		}
	}
	return gamesCounter;
}

int countGamesInSection() {
	int gamesCounter=0;
	for (int i=0;i<=totalPages;i++) {
		for (int j=0;j<ITEMS_PER_PAGE;j++) {
			if (gameList[i][j]!=NULL) {
				gamesCounter++;
			}
		}
	}
	return gamesCounter;
}

void determineStartingScreen(int sectionCount) {
	if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
		favoritesSectionSelected=1;
		loadFavoritesList();
	} else {
		if(CURRENT_SECTION.hidden) {
			int startingSectionNumber = currentSectionNumber;
			int stillOnInitialSection=0;
			int rewinded = rewindSection();
			if(rewinded) {
				while(menuSections[currentSectionNumber].hidden) {
					if(currentSectionNumber==0) {
						stillOnInitialSection=1;
						break;
					}
					rewindSection();
				}
				if (stillOnInitialSection) {
					currentSectionNumber = startingSectionNumber;
				}
				loadGameList();
			}
			if(currentSectionNumber==startingSectionNumber) {
				int advanced = advanceSection();
				if(advanced) {
					while(menuSections[currentSectionNumber].hidden) {
						advanceSection();
					}
					loadGameList();
				}
			}
		}
		loadGameList();
	}
}
