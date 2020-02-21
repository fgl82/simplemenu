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
#include "../headers/doubly_linked_rom_list.h"

void scrollDown() {
	CURRENT_SECTION.currentGameNode=CURRENT_SECTION.currentGameNode->next;
	// if at the end end of the list
	if (CURRENT_SECTION.currentGameNode==NULL) {
		//restart pages
		CURRENT_SECTION.currentGameNode=CURRENT_SECTION.head;
		CURRENT_SECTION.currentPage=0;
		CURRENT_SECTION.currentGameInPage=0;
	} else {
		// it's a filled page and not the last item
		if (CURRENT_SECTION.currentGameInPage < ITEMS_PER_PAGE-1) {
			// increase selected game
			CURRENT_SECTION.currentGameInPage++;
		} else {
			CURRENT_SECTION.currentPage++;
			CURRENT_SECTION.currentGameInPage=0;
		}
	}
	CURRENT_SECTION.realCurrentGameNumber=CURRENT_GAME_NUMBER;
}

int advanceSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber<favoritesSectionNumber-1) {
		currentSectionNumber++;
	} else if (currentSectionNumber!=favoritesSectionNumber) {
		currentSectionNumber=0;
	}
	if (pictureMode) {
		if(theCurrentSectionHasGames()) {
			displayBackgroundPicture();
			showConsole();
			refreshScreen();
		}
	}
	if(currentSectionNumber!=favoritesSectionNumber) {
		loadGameList(0);
		int number = CURRENT_SECTION.realCurrentGameNumber;
		int gamesInSection=CURRENT_SECTION.gameCount;
		int pages = gamesInSection / ITEMS_PER_PAGE;
		if (gamesInSection%ITEMS_PER_PAGE==0) {
			pages--;
		}
		CURRENT_SECTION.totalPages=pages;
		CURRENT_SECTION.currentGameInPage=0;
		CURRENT_SECTION.currentPage=0;
		while (CURRENT_GAME_NUMBER<number) {
			gamesInPage=countGamesInPage();
			scrollDown();
		}
		return 1;
	}
	return 0;
}

int rewindSection() {
	if(currentSectionNumber!=favoritesSectionNumber&&currentSectionNumber>0) {
		currentSectionNumber--;
	} else if (currentSectionNumber!=favoritesSectionNumber) {
		currentSectionNumber=menuSectionCounter-2;
	}
	if (pictureMode) {
		if(theCurrentSectionHasGames()) {
			displayBackgroundPicture();
			showConsole();
			refreshScreen();
		}
	}
	if(currentSectionNumber!=favoritesSectionNumber) {
		loadGameList(0);
		printf("ES: %s\n", "PAPAPA");
		int number = CURRENT_SECTION.realCurrentGameNumber;
		int gamesInSection=CURRENT_SECTION.gameCount;
		int pages = gamesInSection / ITEMS_PER_PAGE;
		if (gamesInSection%ITEMS_PER_PAGE==0) {
			pages--;
		}
		CURRENT_SECTION.totalPages=pages;
		CURRENT_SECTION.currentGameInPage=0;
		CURRENT_SECTION.currentPage=0;
		while (CURRENT_GAME_NUMBER<number) {
			gamesInPage=countGamesInPage();
			printf("ES: %s\n", "PAPAPA3");
			scrollDown();
		}
		printf("ES: %s\n", "PAPAPA2");
		return 1;
	}
	return 0;
}

void showPicture() {
	displayGamePicture();
}

void launchGame(struct Rom *rom) {
	if (favoritesSectionSelected && favoritesSize > 0) {
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		executeCommand(favorite.emulatorFolder,favorite.executable,favorite.name);
	} else if (rom->name!=NULL) {
		if (CURRENT_SECTION.onlyFileNamesNoExtension) {
			executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],getGameName(rom->name));
		} else {
			executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],rom->name);
		}
	}
}

void launchEmulator(struct Rom *rom) {
	if (favoritesSectionSelected && favoritesSize > 0) {
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		executeCommand(favorite.emulatorFolder,favorite.executable,"*");
	} else if (rom->name!=NULL) {
		executeCommand(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory], CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable],"*");
	}
}

void scrollUp() {
	CURRENT_SECTION.currentGameNode=CURRENT_SECTION.currentGameNode->prev;
	//if at the first item of the first page
	if (CURRENT_SECTION.currentGameNode==NULL) {
		//go to the last page
		CURRENT_SECTION.currentPage=CURRENT_SECTION.totalPages;
		//go to the last game
		if (CURRENT_SECTION.totalPages==0) {
			CURRENT_SECTION.currentGameInPage=CURRENT_SECTION.gameCount-1;
		} else {
			printf("%d = %d\n",CURRENT_SECTION.gameCount,(ITEMS_PER_PAGE));
			if (CURRENT_SECTION.gameCount%(ITEMS_PER_PAGE)!=0) {
				printf("NOT SAME\n");
				CURRENT_SECTION.currentGameInPage=(CURRENT_SECTION.gameCount%ITEMS_PER_PAGE)-1;
			} else {
				CURRENT_SECTION.currentGameInPage=ITEMS_PER_PAGE-1;
			}
		}
		//point to the last node
		CURRENT_SECTION.currentGameNode=CURRENT_SECTION.tail;
	} else {
		//it's a full page and not the first item
		if (CURRENT_SECTION.currentGameInPage > 0) {
			//decrease selected game
			CURRENT_SECTION.currentGameInPage--;
		} else {
			//decrease page
			CURRENT_SECTION.currentPage--;
			//go to the last ekement of the page
			CURRENT_SECTION.currentGameInPage=ITEMS_PER_PAGE-1;
		}
	}
	//establish real game number
	CURRENT_SECTION.realCurrentGameNumber=CURRENT_GAME_NUMBER;
}

void advancePage(struct Rom *rom) {
	if(CURRENT_SECTION.currentPage<=CURRENT_SECTION.totalPages) {
		if (rom==NULL||rom->name==NULL) {
			return;
		}
		if (CURRENT_SECTION.alphabeticalPaging) {
			char *currentGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->data);
			char currentLetter=tolower(currentGame[0]);
			while((tolower(currentGame[0])==currentLetter||isdigit(currentGame[0]))) {
				if (CURRENT_SECTION.currentPage==CURRENT_SECTION.totalPages&&CURRENT_SECTION.currentGameInPage==countGamesInPage()-1) {
					scrollDown();
					break;
				}
				scrollDown();
				free(currentGame);
				currentGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->data);
			}
			free(currentGame);
		} else {
			if(CURRENT_SECTION.currentPage!=CURRENT_SECTION.totalPages) {
				CURRENT_SECTION.currentPage++;
				for (int i=CURRENT_SECTION.currentGameInPage;i<ITEMS_PER_PAGE;i++) {
					CURRENT_SECTION.currentGameNode=CURRENT_SECTION.currentGameNode->next;
				}
			} else {
				CURRENT_SECTION.currentPage=0;
				CURRENT_SECTION.currentGameNode=CURRENT_SECTION.head;
			}
			CURRENT_SECTION.currentGameInPage=0;
		}
	}
	gamesInPage=countGamesInPage();
	CURRENT_SECTION.realCurrentGameNumber=CURRENT_GAME_NUMBER;
}

void rewindPage(struct Rom *rom) {
//	struct Node* currentGameNode = getCurrentNode();
	if (rom==NULL||rom->name==NULL) {
		return;
	}
	if (CURRENT_SECTION.alphabeticalPaging) {
		char *currentGame = getFileNameOrAlias(rom);
		char *previousGame;
		int hitStart = 0;
		while(!(CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGameInPage==0)) {
			previousGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->prev->data);
			if(tolower(currentGame[0])==tolower(previousGame[0])) {
				if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGameInPage==0) {
					hitStart = 1;
					break;
				} else {
					scrollUp();
				}
				free(currentGame);
				free(previousGame);
				currentGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->data);
			} else {
				break;
			}

		}
		if (!hitStart) {
			scrollUp();
		}
		hitStart=0;
		free(currentGame);
		currentGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->data);
		while(!(CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGameInPage==0)) {
			previousGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->prev->data);
			if ( (tolower(currentGame[0])==tolower(previousGame[0])) ||
					(isdigit(currentGame[0])&&isdigit(previousGame[0]))) {
				if (CURRENT_SECTION.currentPage==0&&CURRENT_SECTION.currentGameInPage==0) {
					hitStart = 1;
					break;
				} else {
					scrollUp();
				}
				free(currentGame);
				free(previousGame);
				currentGame = getFileNameOrAlias(CURRENT_SECTION.currentGameNode->data);
			} else {
				break;
			}
		}
		free(currentGame);
	} else if (CURRENT_SECTION.currentPage > 0) {
		int tempCurrentPage = CURRENT_SECTION.currentPage;
		while (!(CURRENT_SECTION.currentPage==(tempCurrentPage-1)&&CURRENT_SECTION.currentGameInPage==0)) {
			scrollUp();
		}
	} else {
		while (!(CURRENT_SECTION.currentPage==CURRENT_SECTION.totalPages&&CURRENT_SECTION.currentGameInPage==0)) {
			scrollUp();
		}
	}
	gamesInPage=countGamesInPage();
	CURRENT_SECTION.realCurrentGameNumber=CURRENT_GAME_NUMBER;
}

void showOrHideFavorites() {
	hideFullScreenModeMenu();
	if (favoritesSectionSelected) {
		favoritesSectionSelected=0;
		currentSectionNumber=returnTo;
		if (returnTo==0) {
			if (pictureMode) {
				currentlySectionSwitching=1;
				resetPicModeHideLogoTimer();
				currentlySectionSwitching=1;
				displayBackgroundPicture();
				showConsole();
//				refreshScreen();
			}
			determineStartingScreen(menuSectionCounter);
		} else {
			if (pictureMode) {
				currentlySectionSwitching=1;
				resetPicModeHideLogoTimer();
				displayBackgroundPicture();
				showConsole();
//				refreshScreen();
			}
			loadGameList(0);
			int number = CURRENT_SECTION.realCurrentGameNumber;
			int gamesInSection=countGamesInSection();
			int pages = gamesInSection / ITEMS_PER_PAGE;
			if (gamesInSection%ITEMS_PER_PAGE==0) {
				pages--;
			}
			CURRENT_SECTION.totalPages=pages;
			CURRENT_SECTION.currentGameInPage=0;
			CURRENT_SECTION.currentPage=0;
			while (CURRENT_GAME_NUMBER<number) {
				gamesInPage=countGamesInPage();
				scrollDown();
			}
		}
		return;
	}
	if(strlen(favorites[0].name)<2) {
		return;
	}
	favoritesSectionSelected=1;
	returnTo=currentSectionNumber;
	currentSectionNumber=favoritesSectionNumber;

	if (pictureMode) {
		resetPicModeHideLogoTimer();
		currentlySectionSwitching=1;
		displayBackgroundPicture();
		showConsole();
		refreshScreen();
	}

	loadFavoritesSectionGameList();
	int number = CURRENT_SECTION.realCurrentGameNumber;
	int gamesInSection=countGamesInSection();
	int pages = gamesInSection / ITEMS_PER_PAGE;
	if (gamesInSection%ITEMS_PER_PAGE==0) {
		pages--;
	}
	CURRENT_SECTION.totalPages=pages;
	CURRENT_SECTION.currentGameInPage=0;
	CURRENT_SECTION.currentPage=0;
	while (CURRENT_GAME_NUMBER<number) {
		gamesInPage=countGamesInPage();
		scrollDown();
	}
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
		favoritesSize--;
		loadFavoritesSectionGameList();
	}
	if(CURRENT_GAME_NUMBER==favoritesSize) {
		scrollUp();
	}
}

void markAsFavorite(struct Rom *rom) {
	favoritesChanged=1;
	if (favoritesSize<FAVORITES_SIZE) {
		if (!doesFavoriteExist(rom->name)) {
			#ifdef TARGET_RG350
			Shake_Play(device, effect_id);
			#endif		
			if (CURRENT_SECTION.onlyFileNamesNoExtension) {
				strcpy(favorites[favoritesSize].name, getGameName(rom->name));
			} else {
				strcpy(favorites[favoritesSize].name, rom->name);
			}
			if(rom->alias!=NULL&&strlen(rom->alias)>2) {
				strcpy(favorites[favoritesSize].alias, rom->alias);
			} else {
				favorites[favoritesSize].alias[0]=' ';
			}
			strcpy(favorites[favoritesSize].section,CURRENT_SECTION.sectionName);
			strcpy(favorites[favoritesSize].emulatorFolder,CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory]);
			strcpy(favorites[favoritesSize].executable,CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable]);
			strcpy(favorites[favoritesSize].filesDirectory,rom->directory);
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

void callDeleteGame(struct Rom *rom) {
	if (!favoritesSectionSelected) {
		deleteGame(rom);
		loadGameList(1);
		while(CURRENT_SECTION.hidden) {
			rewindSection();
			loadGameList(0);
			hideFullScreenModeMenu();
		}
		if(CURRENT_SECTION.currentGameInPage==countGamesInPage()) {
			scrollUp();
		}
		setupDecorations();
	} else {
		generateError("YOU CAN'T DELETE GAMES-WHILE IN FAVORITES",0);
	}
}
