#include <config.h>
#include <constants.h>
#include <definitions.h>
#include <globals.h>
#include <logic.h>
#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system_logic.h>
#include <SDL/SDL_keysym.h>

void changePaging() {
	CURRENT_SECTION.alphabeticalPaging=1+(CURRENT_SECTION.alphabeticalPaging*-1);
	setupDecorations();
	totalPages=0;
	CURRENT_SECTION.currentGame=0;
	CURRENT_SECTION.currentPage=0;
	if (favoritesSectionSelected) {
		loadFavoritesList();
	} else {
		loadGameList();
	}
}

int advanceSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber<favoritesSectionNumber-1) {
		currentSectionNumber++;
		return 1;
	}
	return 0;
}

int rewindSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber>0) {
		currentSectionNumber--;
		return 1;
	}
	return 0;
}

void showPicture() {
	displayGamePicture();
}

void launchGame() {
	char fileToBeExecutedwithFullPath[200];
	if (favoritesSectionSelected && favoritesSize > 0) {
		strcpy(fileToBeExecutedwithFullPath,CURRENT_FAVORITE.filesDirectory);
		strcat(fileToBeExecutedwithFullPath,CURRENT_GAME_NAME);
		executeCommand(CURRENT_FAVORITE.emulatorFolder,CURRENT_FAVORITE.executable,fileToBeExecutedwithFullPath);
	} else if (CURRENT_GAME_NAME!=NULL) {
		strcpy(fileToBeExecutedwithFullPath,CURRENT_SECTION.filesDirectory);
		strcat(fileToBeExecutedwithFullPath,CURRENT_GAME_NAME);
		executeCommand(CURRENT_SECTION.emulatorFolder, CURRENT_SECTION.executable,fileToBeExecutedwithFullPath);
	}
}

void scrollUp() {
	if(CURRENT_SECTION.currentGame == 0) {
		if (CURRENT_SECTION.currentPage >0) {
			CURRENT_SECTION.currentPage--;
			gamesInPage=countGamesInPage();
			CURRENT_SECTION.currentGame=gamesInPage-1;
			return;
		}
	}
	if (CURRENT_SECTION.currentGame > 0) {
		CURRENT_SECTION.currentGame--;
		return;
	}
}

void scrollDown() {
	if(CURRENT_SECTION.currentGame == gamesInPage-1) {
		if (CURRENT_SECTION.currentPage < totalPages) {
			CURRENT_SECTION.currentGame=0;
			CURRENT_SECTION.currentPage++;
			return;
		}
	}
	if (CURRENT_SECTION.currentGame < gamesInPage-1) {
		CURRENT_SECTION.currentGame++;
		return;
	}
}

void advancePage() {
	if(CURRENT_SECTION.currentPage<=totalPages) {
		if (CURRENT_SECTION.alphabeticalPaging) {
			char currentLetter=CURRENT_GAME_NAME[0];
			while(CURRENT_GAME_NAME[0]==currentLetter||isdigit(CURRENT_GAME_NAME[0])) {
				scrollDown();
				if (CURRENT_SECTION.currentPage==totalPages&&CURRENT_SECTION.currentGame==countGamesInPage()-1) {
					break;
				}
			}
		} else {
			if(CURRENT_SECTION.currentPage!=totalPages) {
				CURRENT_SECTION.currentPage++;
				CURRENT_SECTION.currentGame=0;
			}
		}
	}
}

void rewindPage() {
	if (CURRENT_SECTION.alphabeticalPaging) {
		char currentLetter=CURRENT_GAME_NAME[0];
		int hitStart = 0;
		int wasDigit = isdigit(CURRENT_GAME_NAME[0]);
		while(CURRENT_GAME_NAME[0]==currentLetter||(wasDigit&&isdigit(CURRENT_GAME_NAME[0]))) {
			scrollUp();
			if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0) {
				hitStart = 1;
				break;
			}
		}
		if (!hitStart) {
			wasDigit = isdigit(CURRENT_GAME_NAME[0]);
			currentLetter=CURRENT_GAME_NAME[0];
			while(CURRENT_GAME_NAME[0]==currentLetter||(wasDigit&&isdigit(CURRENT_GAME_NAME[0]))) {
				scrollUp();
				if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0) {
					hitStart = 1;
					break;
				}
			}
			printf("papa\n");
			if (!hitStart) {
				scrollDown();
			}
		}
		gamesInPage=countGamesInPage();
	} else 	if (CURRENT_SECTION.currentPage > 0) {
		CURRENT_SECTION.currentPage--;
		gamesInPage=countGamesInPage();
		CURRENT_SECTION.currentGame=0;
	}
}

void showOrHideFavorites() {
	if (favoritesSectionSelected) {
		favoritesSectionSelected=0;
		currentSectionNumber=returnTo;
		setupDecorations();
		totalPages=0;
		loadGameList();
		return;
	}
	favoritesSectionSelected=1;
	returnTo=currentSectionNumber;
	currentSectionNumber=favoritesSectionNumber;
	setupDecorations();
	totalPages=0;
	loadFavoritesList();
}

void removeFavorite() {
	favoritesChanged=1;
	if (favoritesSize>0) {
		for (int i=CURRENT_FAVORITE_NUMBER;i<favoritesSize;i++) {
			favorites[i]=favorites[i+1];
		}
		favoritesSize--;
		scrollUp();
		setupDecorations();
		totalPages=0;
		loadFavoritesList();
	}
}

void markAsFavorite() {
	favoritesChanged=1;
	if (favoritesSize<FAVORITES_SIZE) {
		if (!doesFavoriteExist(CURRENT_GAME_NAME)) {
			strcpy(favorites[favoritesSize].name, CURRENT_GAME_NAME);
			strcpy(favorites[favoritesSize].emulatorFolder,CURRENT_SECTION.emulatorFolder);
			strcpy(favorites[favoritesSize].executable,CURRENT_SECTION.executable);
			strcpy(favorites[favoritesSize].filesDirectory,CURRENT_SECTION.filesDirectory);
			favoritesSize++;
			loadFavoritesList();
			loadGameList();
		}
	}
}

int performAction() {
	if (keys[BTN_SELECT] && keys[BTN_START]) {
		running=0;
		return 0;
	}
	if (keys[BTN_TA] && keys[BTN_START]) {
		freeResources();
		saveLastState();
		saveFavorites();
		exit(0);
	}
	if (keys[BTN_START]) {
		if (!favoritesSectionSelected) {
			markAsFavorite();
		} else {
			removeFavorite();
		}
		return 0;
	}
	if (keys[BTN_R]) {
		showOrHideFavorites();
		return 0;
	}
	if(keys[BTN_SELECT]) {
		if (keys[BTN_RIGHT]) {
			CURRENT_SECTION.alphabeticalPaging=1;
			advancePage();
			CURRENT_SECTION.alphabeticalPaging=0;
			return 0;
		}
		if (keys[BTN_LEFT]) {
			CURRENT_SECTION.alphabeticalPaging=1;
			rewindPage();
			CURRENT_SECTION.alphabeticalPaging=0;
			return 0;
		}
		if (keys[BTN_UP]) {
			cycleFrequencies();
			drawHeader();
			return 0;
		}
	}
	if(keys[BTN_TA]) {
		int startingSectionNumber = currentSectionNumber;
		int wasLastSectionWithContent=0;
		int advanced = advanceSection();
		if(advanced) {
			while(menuSections[currentSectionNumber].hidden) {
				if(currentSectionNumber==favoritesSectionNumber-1) {
					wasLastSectionWithContent=1;
					break;
				}
				advanceSection();
			}
			if (wasLastSectionWithContent) {
				currentSectionNumber = startingSectionNumber;
			}
			setupDecorations();
			totalPages=0;
			loadGameList();
		}
		return 0;
	}
	if(keys[BTN_TB]) {
		int startingSectionNumber = currentSectionNumber;
		int wasFirstSectionWithContent=0;
		int rewinded = rewindSection();
		if(rewinded) {
			while(menuSections[currentSectionNumber].hidden) {
				if(currentSectionNumber==0) {
					wasFirstSectionWithContent=1;
					break;
				}
				rewindSection();
			}
			if (wasFirstSectionWithContent) {
				currentSectionNumber = startingSectionNumber;
			}
			setupDecorations();
			totalPages=0;
			loadGameList();
		}
		return 0;
	}
	if (keys[BTN_A]) {
		if (countGamesInPage()>0) {
			saveFavorites();
			freeResources();
			launchGame();
		}
		return 0;
	}
	if (keys[BTN_B]) {
		if (pictureMode) {
			pictureMode=0;
			setupDecorations();
		} else {
			pictureMode=1;
		}
	}
	if (keys[BTN_DOWN]) {
		scrollDown();
		return 1;
	}
	if(keys[BTN_UP]) {
		scrollUp();
		return 1;
	}
	if(keys[BTN_RIGHT]) {
		advancePage();
		return 1;
	}
	if(keys[BTN_LEFT]) {
		rewindPage();
		return 1;
	}
	return 0;
}
