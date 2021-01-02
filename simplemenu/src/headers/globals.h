#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

#include <pthread.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_image.h>
#include "../headers/hashtable.h"
#include "../headers/constants.h"

#ifdef TARGET_RG350
#include <shake.h>
#endif

/* STATUS */
extern char *errorMessage;
extern int running;
extern int itsStoppedBecauseOfAnError;
extern int thereIsACriticalError;
extern int favoritesChanged;
extern int favoritesSectionSelected;
extern int returnTo;
extern int currentSectionNumber;
extern int currentCPU;
extern int fullscreenMode;
extern int hotKeyPressed;
extern int aKeyComboWasPressed;
extern int currentlySectionSwitching;
extern int currentlyChoosing;
extern int loading;
extern int isPicModeMenuHidden;
extern int isSuspended;
extern int isUSBMode;
extern int activeGroup;
extern int beforeTryingToSwitchGroup;
extern int chosenSetting;
extern char currentGameNameBeingDisplayed [3000];
extern SDL_TimerID timeoutTimer;
extern SDL_TimerID picModeHideMenuTimer;
extern SDL_TimerID picModeHideLogoTimer;
extern SDL_TimerID hideHeartTimer;

typedef struct thread_picture {
	  SDL_Surface* display;
	  SDL_Surface *image;
	  int x;
	  int y;
	  int xx;
	  int yy;
	  double newwidth;
	  double newheight;
	  int transparent;
	  int smoothing;
} threadPicture;

/* QUANTITIES */
extern int MAX_GAMES_IN_SECTION;
extern int ITEMS_PER_PAGE;
extern int FULLSCREEN_ITEMS_PER_PAGE;
extern int MENU_ITEMS_PER_PAGE;
extern int favoritesSectionNumber;
extern int favoritesSize;
extern int menuSectionCounter;
extern int sectionGroupCounter;
extern int gamesInPage;

/* SETTINGS */
extern int TIDY_ROMS_OPTION;
extern int FULL_SCREEN_FOOTER_OPTION;
extern int FULL_SCREEN_MENU_OPTION;
extern int THEME_OPTION;
extern int SCREEN_TIMEOUT_OPTION;
extern int DEFAULT_OPTION;
extern int USB_OPTION;
extern int SHUTDOWN_OPTION;
extern int AUTO_HIDE_LOGOS_OPTION;
extern int ITEMS_PER_PAGE_OPTION;
extern char mediaFolder[1000];
extern int stripGames;
extern int shutDownEnabled;
extern int selectedShutDownOption;
extern int footerVisibleInFullscreenMode;
extern int menuVisibleInFullscreenMode;
extern int autoHideLogos;
extern int timeoutValue;
extern int OC_UC;
extern int OC_NO;
extern int OC_OC;
extern int OC_SLEEP;
extern int backlightValue;
extern int hdmiChanged;
extern pthread_t myThread;

/* THEME */
extern char *themes[100];
extern int activeTheme;
extern int themeCounter;
extern int currentMode;
extern char menuFont[1000];
extern int baseFont;
extern int fontSize;
extern int settingsFontSize;
extern int transparentShading;
extern int footerOnTop;
extern char simpleBackground[1000];
extern char fullscreenBackground[1000];
extern char favoriteIndicator[1000];

extern int itemsInSimple;
extern int itemsInFullSimple;
extern int itemsSeparationInSimple;
extern int gameListPositionInSimple;
extern int gameListPositionInFullSimple;
extern int headerPositionInSimple;
extern int footerPositionInSimple;

extern int itemsInTraditional;
extern int itemsInFullTraditional;
extern int itemsSeparationInTraditional;
extern int gameListPositionInTraditional;
extern int gameListPositionInFullTraditional;
extern int headerPositionInTraditional;
extern int footerPositionInTraditional;

extern int itemsInDrunkenMonkey;
extern int itemsInFullDrunkenMonkey;
extern int itemsSeparationInDrunkenMonkey;
extern int gameListPositionInDrunkenMonkey;
extern int gameListPositionInFullDrunkenMonkey;
extern int headerPositionInDrunkenMonkey;
extern int footerPositionInDrunkenMonkey;

extern int itemsInCustom;
extern int itemsInFullCustom;
extern int itemsSeparationInCustom;
extern char textXFontCustom[1000];
extern int text1FontSizeInCustom;
extern int text1XInCustom;
extern int text1YInCustom;
extern int text1AlignmentInCustom;
extern int text2FontSizeInCustom;
extern int text2XInCustom;
extern int text2YInCustom;
extern int text2AlignmentInCustom;
extern int text3FontSizeInCustom;
extern int text3XInCustom;
extern int text3YInCustom;
extern int text3AlignmentInCustom;
extern int gameListAlignmentInCustom;
extern int gameListXInCustom;
extern int gameListYInCustom;
extern int gameListWidthInCustom;
extern int gameListPositionInFullCustom;
extern int artWidthInCustom;
extern int artHeightInCustom;
extern int artXInCustom;
extern int artYInCustom;
extern int artTextDistanceFromPictureInCustom;
extern int artTextLineSeparationInCustom;
extern int artTextFontSizeInCustom;
extern int systemWidthInCustom;
extern int systemHeightInCustom;
extern int systemXInCustom;
extern int systemYInCustom;
extern int fontSizeCustom;
extern int colorfulFullscreenMenu;
extern int displaySectionGroupName;

/* STRUCTS */
struct OPKDesktopFile {
	char parentOPK[200];
	char name[200];
	char displayName[200];
	char category[200];
};

struct StolenGMenuFile {
	char title[200];
	char exec[200];
};

struct Favorite {
	char section[300];
	char name[300];
	char alias[300];
	char emulatorFolder[200];
	char executable[200];
	char filesDirectory[400];
};

struct Rom {
	char *name;
	char *alias;
	char *directory;
};

struct Node  {
	struct Rom  *data;
	struct Node *next;
	struct Node *prev;
};

struct SectionGroup {
	char groupPath[1000];
	char groupName[25];
	char groupBackground[1000];
	SDL_Surface *groupBackgroundSurface;
};

struct MenuSection {
	char sectionName[25];
	char *emulatorDirectories[10];
	char *executables[10];
	char filesDirectories[400];
	char fileExtensions[150];
	char systemLogo[300];
	SDL_Surface *systemLogoSurface;
	char systemPicture[300];
	SDL_Surface *systemPictureSurface;
	char aliasFileName[300];
	int hidden;
	int currentPage;
	int currentGameInPage;
	int realCurrentGameNumber;
	int alphabeticalPaging;
	int totalPages;
	int gameCount;
	int initialized;
	int onlyFileNamesNoExtension;
	int headerAndFooterBackgroundColor[3];
	int headerAndFooterTextColor[3];
	int bodyBackgroundColor[3];
	int bodyTextColor[3];
	int bodySelectedTextBackgroundColor[3];
	int bodySelectedTextTextColor[3];
	struct Node* currentGameNode;
	struct Node *head;
	struct Node *tail;
	hashtable_t *aliasHashTable;
	int activeExecutable;
	int activeEmulatorDirectory;
	char category[100];
	char scaling[2];
	char background[1000];
	SDL_Surface *backgroundSurface;
	int hasDirs;
//	char *fileList[50000];
};

extern struct SectionGroup sectionGroups[100];
extern int sectionGroupStates[100][100][5];
extern struct MenuSection menuSections[100];
extern struct Favorite favorites[2000];

/* CONTROL */
extern uint8_t *keys;
//extern SDL_Joystick *joystick;
extern int BTN_Y;
extern int BTN_B;
extern int BTN_A;
extern int BTN_X;
extern int BTN_START;
extern int BTN_SELECT;
extern int BTN_R;
extern int BTN_UP;
extern int BTN_DOWN;
extern int BTN_LEFT;
extern int BTN_RIGHT;
extern int BTN_L1;
extern int BTN_R1;
extern int BTN_L2;
extern int BTN_R2;

#ifdef TARGET_RG350
extern Shake_Device *device;
extern Shake_Effect effect;
extern int effect_id;
extern Shake_Effect effect1;
extern int effect_id1;
#endif

/* SCREEN */
extern int MAGIC_NUMBER;
extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;
extern double SCREEN_RATIO;
extern int hdmiEnabled;

/* MISC */
extern time_t currRawtime;
extern struct tm * currTime;
extern int lastSec;
extern int lastMin;
extern int lastChargeLevel;
extern pthread_t clockThread;
extern pthread_mutex_t lock;

#endif
