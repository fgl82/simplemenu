#ifndef DEFINITIONS_DEFINED
#define DEFINITIONS_DEFINED
#define SCREEN_WIDTH   (SCREEN_HEIGHT/3)*4
#define SCREEN_HEIGHT   240
#define BTN_X			SDLK_SPACE
#define BTN_B			SDLK_LCTRL
#define BTN_A			SDLK_LALT
#define BTN_Y			SDLK_LSHIFT
#define BTN_START		SDLK_RETURN
#define BTN_SELECT		SDLK_ESCAPE
#define BTN_R  		    SDLK_END
#define BTN_UP			SDLK_UP
#define BTN_DOWN		SDLK_DOWN
#define BTN_LEFT		SDLK_LEFT
#define BTN_RIGHT		SDLK_RIGHT
#define BTN_LB			SDLK_TAB
#define BTN_RB			SDLK_BACKSPACE
#define FAVORITES_SIZE 2000
#define CURRENT_GAME_NUMBER menuSections[currentSectionNumber].currentPage*10+menuSections[currentSectionNumber].currentGame
#define CURRENT_FAVORITE_NAME_IN_FAVORITES_LIST favorites[CURRENT_GAME_NUMBER]
#define CURRENT_SECTION menuSections[currentSectionNumber]
#define FAVORITES_SECTION menuSections[favoritesSectionNumber]
#define NEXT_SECTION menuSections[currentSectionNumber+1]
#define CURRENT_GAME_NAME CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][menuSections[currentSectionNumber].currentGame]
#define PREVIOUS_GAME_NAME CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][menuSections[currentSectionNumber].currentGame-1]
#define NEXT_GAME_NAME CURRENT_SECTION.gameList[menuSections[currentSectionNumber].currentPage][menuSections[currentSectionNumber].currentGame+1]
#endif
