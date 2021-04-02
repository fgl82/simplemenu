#include <stdlib.h>
#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/system_logic.h"
#include "../headers/utils.h"

int performAction(struct Node *node) {
	struct Rom *rom;
	if (node!=NULL) {
		rom = node->data;
	} else {
		rom = NULL;
	}
	if(currentlySectionSwitching) {
		if (keys[BTN_A]) {
			currentlySectionSwitching=0;
			if (CURRENT_SECTION.backgroundSurface==NULL) {
				logMessage("INFO","Loading system background");
				CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
				resizeSectionBackground(&CURRENT_SECTION);
				logMessage("INFO","Loading system picture");
				CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
				resizeSectionSystemPicture(&CURRENT_SECTION);
			}
			if (keys[BTN_START]) {
				currentlySectionSwitching=0;
				chosenSetting=SHUTDOWN_OPTION;
				selectedShutDownOption=0;
				currentState=3;
//				pthread_create(&clockThread, NULL, updateClock,NULL);
				return 1;
			}
			return 1;
		}
	}
	if(itsStoppedBecauseOfAnError&&!keys[BTN_A]) {
		return(0);
	}
	if(keys[BTN_B]) {
		hotKeyPressed=1;
		if (rom!=NULL&&keys[BTN_R]) {
			hideFullScreenModeMenu();
			if(currentSectionNumber!=favoritesSectionNumber) {
				loadGameList(1);
				return(1);
			}
		}
		if (rom!=NULL&&keys[BTN_A]&&!currentlySectionSwitching) {
			launchEmulator(rom);
			aKeyComboWasPressed=1;
			return 1;
		}
		if (rom!=NULL&&keys[BTN_X]&&!currentlySectionSwitching) {
			if (!isPicModeMenuHidden) {
				resetPicModeHideMenuTimer();
			}
			callDeleteGame(rom);
			aKeyComboWasPressed=1;
			return 1;
		}

		if (rom!=NULL&&keys[BTN_Y]) {
			hideFullScreenModeMenu();
			if(currentSectionNumber!=favoritesSectionNumber) {
				loadGameList(1);
				return(1);
			}
		}

		if (keys[BTN_START]&&!currentlySectionSwitching) {
			hotKeyPressed=0;
			cycleFrequencies();
			aKeyComboWasPressed=1;
			return 0;
		}
		if (rom!=NULL&&keys[BTN_SELECT]&&!currentlySectionSwitching) {
			for(int i=0;i<25;i++) {
				selectRandomGame();
			}
			saveFavorites();
			launchGame(CURRENT_SECTION.currentGameNode->data);
		}
		if (rom!=NULL&&keys[BTN_DOWN]&&!currentlySectionSwitching) {
			hotKeyPressed=1;
			CURRENT_SECTION.alphabeticalPaging=1;
			advancePage(rom);
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			aKeyComboWasPressed=1;
			return 0;
		}
		if (rom!=NULL&&keys[BTN_UP]&&!currentlySectionSwitching) {
			hotKeyPressed=1;
			CURRENT_SECTION.alphabeticalPaging=1;
			rewindPage(rom);
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			aKeyComboWasPressed=1;
			return 0;
		}
		if(keys[BTN_RIGHT]) {
			hotKeyPressed=0;
			currentlySectionSwitching=1;
			int advanced = advanceSection(1);
			if(advanced) {
				if (CURRENT_SECTION.backgroundSurface == NULL) {
					logMessage("INFO","Loading system background");
					CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
					resizeSectionBackground(&CURRENT_SECTION);
					logMessage("INFO","Loading system picture");
					CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
					resizeSectionSystemPicture(&CURRENT_SECTION);
				}
				loadGameList(0);
			}
			if(CURRENT_SECTION.gameCount>0) {
				scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
			}
			aKeyComboWasPressed=1;
			return 0;
		}
		if(keys[BTN_LEFT]) {
			hotKeyPressed=0;
			currentlySectionSwitching=1;
			int rewinded = rewindSection(1);
			if(rewinded) {
				if (CURRENT_SECTION.backgroundSurface == NULL) {
					logMessage("INFO","Loading system background");
					CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
					resizeSectionBackground(&CURRENT_SECTION);
					logMessage("INFO","Loading system picture");
					CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
					resizeSectionSystemPicture(&CURRENT_SECTION);
				}
				loadGameList(0);
			}
			if(CURRENT_SECTION.gameCount>0) {
				scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
			}
			aKeyComboWasPressed=1;
			return 0;
		}
	}
	if (CURRENT_SECTION.executables[1]!=NULL&&keys[BTN_SELECT]&&!favoritesSectionSelected) {
		currentState=1;
		return 0;
	}
	if(keys[BTN_L1]) {
		hotKeyPressed=0;
		if(currentSectionNumber!=favoritesSectionNumber&&autoHideLogos) {
			resetPicModeHideLogoTimer();
			displayLogo=1;
		}
		int rewinded = rewindSection(1);
		if(rewinded) {
			loadGameList(0);
		}
		if(CURRENT_SECTION.gameCount>0) {
			scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
		}
		return 0;
	}

	if(keys[BTN_R1]) {
		hotKeyPressed=0;
		if(currentSectionNumber!=favoritesSectionNumber&&autoHideLogos) {
			resetPicModeHideLogoTimer();
			displayLogo=1;
		}
		int advanced = advanceSection(1);
		if(advanced) {
			loadGameList(0);
		}
		if(CURRENT_SECTION.gameCount>0) {
			scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
		}
		return 0;
	}

	if (!currentlySectionSwitching&&!hotKeyPressed&&!isUSBMode) {

		if (rom!=NULL&&keys[BTN_X]) {
			if(!isPicModeMenuHidden) {
				resetPicModeHideMenuTimer();
			}
			if (!favoritesSectionSelected) {
				markAsFavorite(rom);
			} else {
				removeFavorite();
				if(favoritesSize==0) {
					showOrHideFavorites();
				}
			}
			return 0;
		}
		if (keys[BTN_START]) {
			chosenSetting=SHUTDOWN_OPTION;
			selectedShutDownOption=0;
			currentState=3;
//			pthread_create(&clockThread, NULL, updateClock,NULL);
			return 0;
		}
		if (keys[BTN_R]) {
			showOrHideFavorites();
			return 0;
		}
		if (rom!=NULL&&keys[BTN_A]) {
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
				launchGame(rom);
			}
			return 0;
		}
		if (keys[BTN_Y]) {
			int number = CURRENT_GAME_NUMBER;
			if (fullscreenMode) {
				fullscreenMode=0;
				ITEMS_PER_PAGE=MENU_ITEMS_PER_PAGE;
 			} else {
				fullscreenMode=1;
				ITEMS_PER_PAGE=FULLSCREEN_ITEMS_PER_PAGE;
			}
			if (CURRENT_SECTION.gameCount>0) {
				scrollToGame(number);
			}
		}
		if (rom!=NULL&&keys[BTN_DOWN]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			scrollDown();
			return 1;
		}
		if(rom!=NULL&&keys[BTN_UP]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			scrollUp();
			return 1;
		}
		if(rom!=NULL&&keys[BTN_RIGHT]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			advancePage(rom);
			return 1;
		}
		if(rom!=NULL&&keys[BTN_LEFT]) {
			if(fullscreenMode) {
				resetPicModeHideMenuTimer();
			}
			rewindPage(rom);
			return 1;
		}
	}
	return 0;
}
