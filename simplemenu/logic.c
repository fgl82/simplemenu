#include <config.h>
#include <constants.h>
#include <control.h>
#include <definitions.h>
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

int isExtensionValid(char *extension, struct MenuSection section) {
	return(strcmp(extension,section.fileExtension));
}

int countFiles (char* directoryName) {
	struct dirent **files;
	return scandir(directoryName, &files, 0, alphasort);
}

void sortFavorites() {
    struct Favorite tmp;
    for(int i=0; i<favoritesSize; i++) {
        for(int j = 0; j<favoritesSize; j++) {
            if(strcmp(favorites[i].name, favorites[j].name) < 0) {
                tmp = favorites[i];
                favorites[i] = favorites[j];
                favorites[j] = tmp;
            }
        }
    }
}

void loadFavoritesList() {
	int game = 0;
	int page = 0;
	for (int i=0;i<2000;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	char currentLetter;
	char previousLetter='\0';
	for (int i=0;i<favoritesSize;i++){
		currentLetter=favorites[i].name[0];
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize-1) {
				page++;
				totalPages++;
				game = 0;
			}
		}
		gameList[page][game] = favorites[i].name;
		game++;
		previousLetter=currentLetter;
	}
	sortFavorites();
}

void loadGameList() {
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
	char currentLetter;
	char previousLetter='\0';
	for (int i=0;i<n;i++){
		if (strcmp((files[i]->d_name),".gitignore")!=0 &&
				strcmp((files[i]->d_name),"..")!=0 &&
				strcmp((files[i]->d_name),".")!=0 &&
				isExtensionValid(getExtension((files[i]->d_name)),CURRENT_SECTION)==0){
			currentLetter=files[i]->d_name[0];
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
			previousLetter=currentLetter;
		}
	}
	if (lastRound==1) {
		totalPages--;
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
				setupDecorations();
				totalPages=0;
				loadGameList();
			}
			if(currentSectionNumber==startingSectionNumber) {
				int advanced = advanceSection();
				if(advanced) {
					while(menuSections[currentSectionNumber].hidden) {
						advanceSection();
					}
					setupDecorations();
					totalPages=0;
					loadGameList();
				}
			}
		}
		loadGameList();
	}
}
