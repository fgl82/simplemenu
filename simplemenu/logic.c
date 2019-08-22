#include <constants.h>
#include <config.h>
#include <dirent.h>
#include <globals.h>
#include <definitions.h>
#include <screen.h>
#include <control.h>
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

void executeCommand (char *emulatorFolder, char *executable, char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	strcpy(command, "cd ");
	if (favoritesSectionSelected) {
		strcat(command, emulatorFolder);
	} else {
		strcat(command, emulatorFolder);
	}
	strcat(command, ";");
	if (strcmp(executable,"none")!=0) {
		strcat(command, "./");
		strcat(command, executable);
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

int isExtensionValid(char *extension, struct MenuSection section) {
	if(currentSectionNumber>0) {
		return(strcmp(extension,section.fileExtension));
	}
	return 0;
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
	for (int i=0;i<200;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<favoritesSize;i++){
		gameList[page][game] = favorites[i].name;
		game++;
		if (game==ITEMS_PER_PAGE) {
			if(i!=favoritesSize-1) {
				page++;
				totalPages++;
				game = 0;
			}
		}
	}
	sortFavorites();
}

void loadGameList() {
	struct dirent **files;
	int n=scandir(CURRENT_SECTION.filesDirectory, &files, 0, alphasort);
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
				isExtensionValid(getExtension((files[i]->d_name)),CURRENT_SECTION)==0){
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