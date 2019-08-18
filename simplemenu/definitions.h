#ifndef DEFINITIONS_DEFINED
#define DEFINITIONS_DEFINED
#define SCREEN_WIDTH   (SCREEN_HEIGHT/3)*4
#define SCREEN_HEIGHT   240
#define BTN_B			SDLK_SPACE
#define BTN_A			SDLK_LCTRL
#define BTN_TA			SDLK_LALT
#define BTN_TB			SDLK_LSHIFT
#define BTN_START		SDLK_RETURN
#define BTN_SELECT		SDLK_ESCAPE
#define BTN_R  		    SDLK_RCTRL
#define BTN_UP			SDLK_UP
#define BTN_DOWN		SDLK_DOWN
#define BTN_LEFT		SDLK_LEFT
#define BTN_RIGHT		SDLK_RIGHT
#define NO_OC 408
#define MED_OC 702
#define MAX_OC 798
#define CURRENT_FAVORITE_NUMBER menuSections[currentSectionNumber].currentPage*10+menuSections[currentSectionNumber].currentGame
#define CURRENT_FAVORITE favorites[CURRENT_FAVORITE_NUMBER]
#define CURRENT_GAME_NAME gameList[menuSections[currentSectionNumber].currentPage][menuSections[currentSectionNumber].currentGame]
#define CURRENT_SECTION menuSections[currentSectionNumber]
#define NEXT_SECTION menuSections[currentSectionNumber+1]
#endif
