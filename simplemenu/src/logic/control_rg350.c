#ifdef TARGET_RG350
#include <shake.h>
#endif
#include <stdlib.h>
#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/logic.h"
#include "../headers/screen.h"

int performAction(struct Rom *rom) {
	if(currentlySectionSwitching) {
		if (keys[BTN_A]) {
			currentlySectionSwitching=0;
			return 1;
		}
	}
	if (rom!=NULL&&keys[BTN_R2]) {
		hideFullScreenModeMenu();
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
		hotKeyPressed=1;
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
		if (keys[BTN_START]&&!currentlySectionSwitching) {
			hotKeyPressed=0;
			int returnedValue = system("./usb_mode_on.sh");
			if (returnedValue==0) {
				isUSBMode = 1;
			} else {
				generateError("USB MODE  NOT AVAILABLE",0);
			}
			aKeyComboWasPressed=1;
		}
		if (rom!=NULL&&keys[BTN_SELECT]&&!currentlySectionSwitching) {
			for(int i=0;i<25;i++) {
				selectRandomGame();
				updateScreen(CURRENT_SECTION.currentGameNode->data);
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
			int advanced = advanceSection(0);
			if(advanced) {
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
			int rewinded = rewindSection(0);
			if(rewinded) {
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
		currentlyChoosing=1;
		return 0;
	}
	if(keys[BTN_L1]) {
		hideFullScreenModeMenu();
		hotKeyPressed=0;
		if (!favoritesSectionSelected) {
			if(autoHideLogos) {
				resetPicModeHideLogoTimer();
			}
			currentlySectionSwitching=1;
		}
		int rewinded = rewindSection(1);
		if(rewinded) {
			currentlySectionSwitching=1;
			loadGameList(0);
//			if (!fullscreenMode) {
//				currentlySectionSwitching=0;
//			}
		}
		if(CURRENT_SECTION.gameCount>0) {
			scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
		}
		return 0;
	}

	if(keys[BTN_R1]) {
		hideFullScreenModeMenu();
		hotKeyPressed=0;
		if (!favoritesSectionSelected) {
			if(autoHideLogos) {
				resetPicModeHideLogoTimer();
			}
			currentlySectionSwitching=1;
		}
		int advanced = advanceSection(1);
		if(advanced) {
			currentlySectionSwitching=1;
			loadGameList(0);
//			if (!fullscreenMode) {
//				currentlySectionSwitching=0;
//			}
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
//			cycleFrequencies();
			chosenSetting=SHUTDOWN_OPTION;
			currentlyChoosing=3;
			return 0;
		}
		if (keys[BTN_L2]) {
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
				ITEMS_PER_PAGE=10;
 			} else {
				fullscreenMode=1;
				ITEMS_PER_PAGE=12;
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
