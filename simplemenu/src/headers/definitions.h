#ifndef DEFINITIONS_DEFINED
#define DEFINITIONS_DEFINED
int SCREEN_HEIGHT;
#define SCREEN_WIDTH   (SCREEN_HEIGHT/3)*4
#if defined(TARGET_BITTBOY)
	#define BTN_Y			SDLK_SPACE
	#define BTN_B			SDLK_LCTRL
	#define BTN_A			SDLK_LALT
	#define BTN_X			SDLK_LSHIFT
	#define BTN_START		SDLK_RETURN
	#define BTN_SELECT		SDLK_ESCAPE
	#define BTN_R  		    SDLK_RCTRL
	#define BTN_UP			SDLK_UP
	#define BTN_DOWN		SDLK_DOWN
	#define BTN_LEFT		SDLK_LEFT
	#define BTN_RIGHT		SDLK_RIGHT
	#define BTN_L1			SDLK_TAB
	#define BTN_R1			SDLK_BACKSPACE
#elif defined(TARGET_RG300)
	#define BTN_X			SDLK_SPACE
	#define BTN_A			SDLK_LCTRL
	#define BTN_B			SDLK_LALT
	#define BTN_Y			SDLK_LSHIFT
	#define BTN_START		SDLK_RETURN
	#define BTN_SELECT		SDLK_ESCAPE
	#define BTN_R  		    SDLK_END
	#define BTN_UP			SDLK_UP
	#define BTN_DOWN		SDLK_DOWN
	#define BTN_LEFT		SDLK_LEFT
	#define BTN_RIGHT		SDLK_RIGHT
	#define BTN_L1			SDLK_TAB
	#define BTN_R1			SDLK_BACKSPACE
#elif defined(TARGET_NPG)
	#define BTN_X			SDLK_SPACE
	#define BTN_A			SDLK_LCTRL
	#define BTN_B			SDLK_LALT
	#define BTN_Y			SDLK_LSHIFT
	#define BTN_START		SDLK_RETURN
	#define BTN_SELECT		SDLK_ESCAPE
	#define BTN_R  		    SDLK_PAGEUP
	#define BTN_L1			SDLK_TAB
	#define BTN_R1			SDLK_BACKSPACE
	#define BTN_L2			SDLK_RSHIFT
	#define BTN_R2			SDLK_PAGEDOWN
	#define BTN_UP			SDLK_UP
	#define BTN_DOWN		SDLK_DOWN
	#define BTN_LEFT		SDLK_LEFT
	#define BTN_RIGHT		SDLK_RIGHT
#else
	#define BTN_X			SDLK_SPACE
	#define BTN_A			SDLK_LCTRL
	#define BTN_B			SDLK_LALT
	#define BTN_Y			SDLK_LSHIFT
	#define BTN_START		SDLK_RETURN
	#define BTN_SELECT		SDLK_ESCAPE
	#define BTN_R  		    SDLK_PAGEUP
	#define BTN_L1			SDLK_TAB
	#define BTN_R1			SDLK_BACKSPACE
	#define BTN_L2			SDLK_PAGEUP
	#define BTN_R2			SDLK_PAGEDOWN
	#define BTN_UP			SDLK_UP
	#define BTN_DOWN		SDLK_DOWN
	#define BTN_LEFT		SDLK_LEFT
	#define BTN_RIGHT		SDLK_RIGHT
#endif

#define FAVORITES_SIZE 2000
#define CURRENT_GAME_NUMBER menuSections[currentSectionNumber].currentPage*ITEMS_PER_PAGE+menuSections[currentSectionNumber].currentGameInPage
#define CURRENT_FAVORITE_NAME_IN_FAVORITES_LIST favorites[CURRENT_GAME_NUMBER]
#define CURRENT_SECTION menuSections[currentSectionNumber]
#define FAVORITES_SECTION menuSections[favoritesSectionNumber]
#define NEXT_SECTION menuSections[currentSectionNumber+1]
#endif
