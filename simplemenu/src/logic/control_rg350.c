#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TARGET_RG350
#include <shake.h>
#endif

#include "../headers/config.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/system_logic.h"

int advanceSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber<favoritesSectionNumber-1) {
		currentSectionNumber++;
		return 1;
	} else if (currentSectionNumber!=favoritesSectionNumber) {
		currentSectionNumber=0;
		return 1;
	}
	return 0;
}

int rewindSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber>0) {
		currentSectionNumber--;
		return 1;
	} else if (currentSectionNumber!=favoritesSectionNumber) {
		currentSectionNumber=menuSectionCounter-2;
		return 1;
	}
	return 0;
}

void showPicture() {
	displayGamePicture();
}

void launchGame() {
	if (favoritesSectionSelected && favoritesSize > 0) {
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		executeCommand(favorite.emulatorFolder,favorite.executable,favorite.name);
	} else if (CURRENT_GAME_NAME!=NULL) {
		if (CURRENT_SECTION.onlyFileNamesNoExtension) {
			printf("ONLY FILE\n");
			executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],getGameName(CURRENT_GAME_NAME));
		} else {
			executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],CURRENT_GAME_NAME);
		}
	}
}

void launchEmulator() {
	if (favoritesSectionSelected && favoritesSize > 0) {
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		executeCommand(favorite.emulatorFolder,favorite.executable,"*");
	} else if (CURRENT_GAME_NAME!=NULL) {
		executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],"*");
	}
}

void scrollUp() {
	if(CURRENT_SECTION.currentGame == 0) {
		if (CURRENT_SECTION.currentPage >0) {
			CURRENT_SECTION.currentPage--;
		} else {
			CURRENT_SECTION.currentPage=CURRENT_SECTION.totalPages;
		}
		gamesInPage=countGamesInPage();
		if (gamesInPage>0) {
			CURRENT_SECTION.currentGame=gamesInPage-1;
		}
		return;
	}
	if (CURRENT_SECTION.currentGame > 0) {
		CURRENT_SECTION.currentGame--;
	}
}

void scrollDown() {
	if (CURRENT_SECTION.currentGame < gamesInPage-1) {
		if (NEXT_GAME!=NULL&&strlen(NEXT_GAME_NAME)>0) {
			CURRENT_SECTION.currentGame++;
		} else {
			CURRENT_SECTION.currentPage=0;
			CURRENT_SECTION.currentGame=0;
		}
	} else {
		if (CURRENT_SECTION.currentPage < CURRENT_SECTION.totalPages) {
			CURRENT_SECTION.currentPage++;
		} else {
			CURRENT_SECTION.currentPage=0;
		}
		CURRENT_SECTION.currentGame=0;
	}
}

void advancePage() {
	if(CURRENT_SECTION.currentPage<=CURRENT_SECTION.totalPages) {
		if (CURRENT_GAME==NULL||CURRENT_GAME_NAME==NULL) {
			return;
		}
		if (CURRENT_SECTION.alphabeticalPaging) {
			char *currentGame = getFileNameOrAlias(CURRENT_GAME);
			char currentLetter=tolower(currentGame[0]);
			while((tolower(currentGame[0])==currentLetter||isdigit(currentGame[0]))) {
				if (CURRENT_SECTION.currentPage==CURRENT_SECTION.totalPages&&CURRENT_SECTION.currentGame==countGamesInPage()-1) {
					scrollDown();
					break;
				}
				scrollDown();
				free(currentGame);
				currentGame = getFileNameOrAlias(CURRENT_GAME);
			}
			free(currentGame);
		} else {
			if(CURRENT_SECTION.currentPage!=CURRENT_SECTION.totalPages) {
				CURRENT_SECTION.currentPage++;
			} else {
				CURRENT_SECTION.currentPage=0;
			}
			CURRENT_SECTION.currentGame=0;
		}
	}
	gamesInPage=countGamesInPage();
}

void rewindPage() {
	if (CURRENT_GAME==NULL||CURRENT_GAME_NAME==NULL) {
		return;
	}
	if (CURRENT_SECTION.alphabeticalPaging) {
		char *currentGame = getFileNameOrAlias(CURRENT_GAME);
		char *previousGame;
		int hitStart = 0;
		while(!(CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0)) {
			previousGame = getFileNameOrAlias(PREVIOUS_GAME);
			if(tolower(currentGame[0])==tolower(previousGame[0])) {
				if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0) {
					hitStart = 1;
					break;
				} else {
					scrollUp();
				}
				free(currentGame);
				free(previousGame);
				currentGame = getFileNameOrAlias(CURRENT_GAME);
			} else {
				break;
			}

		}
		if (!hitStart) {
			scrollUp();
		}
		hitStart=0;
		free(currentGame);
		currentGame = getFileNameOrAlias(CURRENT_GAME);
		while(!(CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0)) {
			previousGame = getFileNameOrAlias(PREVIOUS_GAME);
			if ( (tolower(currentGame[0])==tolower(previousGame[0])) ||
					(isdigit(currentGame[0])&&isdigit(previousGame[0]))) {
				if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGame==0) {
					hitStart = 1;
					break;
				} else {
					scrollUp();
				}
				free(currentGame);
				free(previousGame);
				currentGame = getFileNameOrAlias(CURRENT_GAME);
			} else {
				break;
			}
		}
		free(currentGame);
	} else if (CURRENT_SECTION.currentPage > 0) {
		CURRENT_SECTION.currentPage--;
		CURRENT_SECTION.currentGame=0;
	} else {
		CURRENT_SECTION.currentPage=CURRENT_SECTION.totalPages;
		CURRENT_SECTION.currentGame=0;

	}
	gamesInPage=countGamesInPage();
}

void showOrHideFavorites() {
	if (favoritesSectionSelected) {
		favoritesSectionSelected=0;
		currentSectionNumber=returnTo;
		if (returnTo==0) {
			determineStartingScreen(menuSectionCounter);
		} else {
			loadGameList(0);
		}
		return;
	}
	favoritesSectionSelected=1;
	returnTo=currentSectionNumber;
	currentSectionNumber=favoritesSectionNumber;
	loadFavoritesSectionGameList();
}

void removeFavorite() {
	favoritesChanged=1;
	if (favoritesSize>0) {
		#ifdef TARGET_RG350
		Shake_Play(device, effect_id);
		#endif
		for (int i=CURRENT_GAME_NUMBER;i<favoritesSize;i++) {
			strcpy(favorites[i].emulatorFolder,favorites[i+1].emulatorFolder);
			strcpy(favorites[i].section,favorites[i+1].section);
			strcpy(favorites[i].executable,favorites[i+1].executable);
			strcpy(favorites[i].filesDirectory,favorites[i+1].filesDirectory);
			strcpy(favorites[i].name,favorites[i+1].name);
			strcpy(favorites[i].alias,favorites[i+1].alias);
		}
		strcpy(favorites[favoritesSize-1].section,"\0");
		strcpy(favorites[favoritesSize-1].emulatorFolder,"\0");
		strcpy(favorites[favoritesSize-1].executable,"\0");
		strcpy(favorites[favoritesSize-1].filesDirectory,"\0");
		strcpy(favorites[favoritesSize-1].name,"\0");
		strcpy(favorites[favoritesSize-1].alias,"\0");
		CURRENT_GAME=NULL;
		favoritesSize--;
		loadFavoritesSectionGameList();
	}
	if(CURRENT_GAME==NULL||CURRENT_GAME_NAME==NULL) {
		scrollUp();
	}
}

void markAsFavorite() {
	favoritesChanged=1;
	if (favoritesSize<FAVORITES_SIZE) {
		if (!doesFavoriteExist(CURRENT_GAME_NAME)) {
			#ifdef TARGET_RG350
			Shake_Play(device, effect_id);
			#endif
			if (CURRENT_SECTION.onlyFileNamesNoExtension) {
				strcpy(favorites[favoritesSize].name, getGameName(CURRENT_GAME_NAME));
			} else {
				strcpy(favorites[favoritesSize].name, CURRENT_GAME_NAME);
			}
			if(CURRENT_GAME->alias!=NULL&&strlen(CURRENT_GAME->alias)>2) {
				strcpy(favorites[favoritesSize].alias, CURRENT_GAME->alias);
			} else {
				favorites[favoritesSize].alias[0]=' ';
			}
			strcpy(favorites[favoritesSize].section,CURRENT_SECTION.sectionName);
			strcpy(favorites[favoritesSize].emulatorFolder,CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory]);
			strcpy(favorites[favoritesSize].executable,CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable]);
			strcpy(favorites[favoritesSize].filesDirectory,CURRENT_GAME->directory);
			favoritesSize++;
			qsort(favorites, favoritesSize, sizeof(struct Favorite), compareFavorites);
		}
	}
}

int isSelectPressed() {
	return keys[BTN_SELECT];
}

void performChoosingAction() {
	if (keys[BTN_UP]) {
		if(CURRENT_SECTION.activeExecutable>0) {
			CURRENT_SECTION.activeExecutable--;
			CURRENT_SECTION.activeEmulatorDirectory--;
		} else {
			CURRENT_SECTION.activeExecutable=sizeof(CURRENT_SECTION.executables)/sizeof(CURRENT_SECTION.executables[0])-1;
			while (CURRENT_SECTION.activeExecutable>0&&CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable]==NULL) {
				CURRENT_SECTION.activeExecutable--;
				CURRENT_SECTION.activeEmulatorDirectory--;
			}
		}
		return;
	}
	if (keys[BTN_DOWN]) {
		if(CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable+1]!=NULL) {
			CURRENT_SECTION.activeExecutable++;
			CURRENT_SECTION.activeEmulatorDirectory++;
		} else {
			CURRENT_SECTION.activeExecutable=0;
			CURRENT_SECTION.activeEmulatorDirectory=0;
		}
		return;
	}
	if (keys[BTN_A]) {
		if (currentlyChoosingEmulator) {
			currentlyChoosingEmulator=0;
			return;
		}
	}
}


int performAction() {
	if (keys[BTN_SELECT] && keys[BTN_START]) {
		running=0;
		return 0;
	}
	if (CURRENT_SECTION.executables[1]!=NULL&&keys[BTN_START]&&!favoritesSectionSelected) {
		currentlyChoosingEmulator=1;
		return 0;
	}
	if (keys[BTN_R2]) {
		if(currentSectionNumber!=favoritesSectionNumber) {
			loadGameList(1);
			return(1);
		}
	}
	if (keys[BTN_START]&&isUSBMode) {
		hotKeyPressed=0;
		isUSBMode=0;
		int ret = system("./usb_mode_off.sh");
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		return 0;
	}
	if(itsStoppedBecauseOfAnError&&!keys[BTN_A]) {
		return(0);
	}
	if(keys[BTN_B]) {
		if (keys[BTN_A]&&!currentlySectionSwitching) {
			launchEmulator();
		}
		if (keys[BTN_X]&&!currentlySectionSwitching) {
			if (!favoritesSectionSelected) {
				deleteCurrentGame(CURRENT_GAME_NAME);
				loadGameList(1);
				while(CURRENT_SECTION.hidden) {
					rewindSection();
					loadGameList(0);
				}
				if(CURRENT_GAME_NAME==NULL) {
					scrollUp();
				}
				setupDecorations();
			} else {
				generateError("YOU CAN'T DELETE GAMES-WHILE IN FAVORITES",0);
				return 1;
			}
		}
		if (keys[BTN_START]&&!currentlySectionSwitching) {
			hotKeyPressed=0;
			int returnedValue = system("./usb_mode_on.sh");
			if (returnedValue==0) {
				isUSBMode = 1;
			} else {
				generateError("USB MODE  NOT AVAILABLE",0);
			}
		}
		if (keys[BTN_SELECT]&&!currentlySectionSwitching) {
			for(int i=0;i<100;i++) {
				selectRandomGame();
				updateScreen();
			}
			saveFavorites();
			launchGame();
		}	
		if (keys[BTN_DOWN]&&!currentlySectionSwitching) {
			hotKeyPressed=1;
			CURRENT_SECTION.alphabeticalPaging=1;
			advancePage();
			CURRENT_SECTION.alphabeticalPaging=0;
			return 0;
		}
		if (keys[BTN_UP]&&!currentlySectionSwitching) {
			hotKeyPressed=1;
			CURRENT_SECTION.alphabeticalPaging=1;
			rewindPage();
			CURRENT_SECTION.alphabeticalPaging=0;
			return 0;
		}
		if(keys[BTN_RIGHT]) {
			hotKeyPressed=0;
			int advanced = advanceSection();
			if(advanced) {
				currentlySectionSwitching=1;
				loadGameList(0);
				while(CURRENT_SECTION.hidden) {
					advanceSection();
					loadGameList(0);
				}
				displayBackgroundPicture();
				showConsole();
				refreshScreen();
			}
			return 0;
		}
		if(keys[BTN_LEFT]) {
			hotKeyPressed=0;
			int rewinded = rewindSection();
			if(rewinded) {
				currentlySectionSwitching=1;
				loadGameList(0);
				while(CURRENT_SECTION.hidden) {
					rewindSection();
					loadGameList(0);
				}
				displayBackgroundPicture();
				showConsole();
				refreshScreen();
			}
			return 0;
		}
	}

	if (!hotKeyPressed&&!currentlySectionSwitching&&!isUSBMode) {
		if(keys[BTN_L1]) {
			hotKeyPressed=0;
			int rewinded = rewindSection();
			if(rewinded) {
				loadGameList(0);
				while(CURRENT_SECTION.hidden) {
					rewindSection();
					loadGameList(0);
				}
			}
			return 0;
		}
		if(keys[BTN_R1]) {
			hotKeyPressed=0;
			int advanced = advanceSection();
			if(advanced) {
				loadGameList(0);
				while(CURRENT_SECTION.hidden) {
					advanceSection();
					loadGameList(0);
				}
			}
			return 0;
		}
		if (keys[BTN_X]) {
			if (!favoritesSectionSelected) {
				markAsFavorite();
			} else {
				removeFavorite();
			}
			return 0;
		}
		if (keys[BTN_START]) {
//			cycleFrequencies();
			return 0;
		}
		if (keys[BTN_R]) {
			showOrHideFavorites();
			return 0;
		}
		if (keys[BTN_A]) {
			if(itsStoppedBecauseOfAnError) {
				if(thereIsACriticalError) {
					#ifndef TARGET_PC
					running=0;
					#else
					freeResources();
					saveLastState();
					saveFavorites();
					exit(0);
					#endif
				}
				itsStoppedBecauseOfAnError=0;
				return 0;
			}
			if (countGamesInPage()>0) {
				saveFavorites();
				launchGame();
			}
			return 0;
		}
		if (keys[BTN_Y]) {
			if (pictureMode) {
				pictureMode=0;
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
	}
	return 0;
}
