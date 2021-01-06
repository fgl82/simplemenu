#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

#include <pthread.h>
#include <SDL/SDL_timer.h>
#include <SDL/SDL_image.h>
#include "../headers/hashtable.h"
#include "../headers/constants.h"

#if defined TARGET_RG350 || defined TARGET_RG350_BETA
#include <shake.h>
#endif

/* STATUS */
 char *errorMessage;
 int running;
 int itsStoppedBecauseOfAnError;
 int thereIsACriticalError;
 int favoritesChanged;
 int favoritesSectionSelected;
 int returnTo;
 int currentSectionNumber;
 int currentCPU;
 int fullscreenMode;
 int hotKeyPressed;
 int aKeyComboWasPressed;
 int currentlySectionSwitching;
 int currentlyChoosing;
 int loading;
 int isPicModeMenuHidden;
 int isSuspended;
 int isUSBMode;
 int activeGroup;
 int beforeTryingToSwitchGroup;
 int chosenSetting;
 char currentGameNameBeingDisplayed [3000];
 SDL_TimerID timeoutTimer;
 SDL_TimerID picModeHideMenuTimer;
 SDL_TimerID picModeHideLogoTimer;
 SDL_TimerID hideHeartTimer;

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
 int MAX_GAMES_IN_SECTION;
 int ITEMS_PER_PAGE;
 int FULLSCREEN_ITEMS_PER_PAGE;
 int MENU_ITEMS_PER_PAGE;
 int favoritesSectionNumber;
 int favoritesSize;
 int menuSectionCounter;
 int sectionGroupCounter;
 int gamesInPage;

/* SETTINGS */
 int TIDY_ROMS_OPTION;
 int FULL_SCREEN_FOOTER_OPTION;
 int FULL_SCREEN_MENU_OPTION;
 int THEME_OPTION;
 int SCREEN_TIMEOUT_OPTION;
 int DEFAULT_OPTION;
 int USB_OPTION;
 int SHUTDOWN_OPTION;
 int AUTO_HIDE_LOGOS_OPTION;
 int ITEMS_PER_PAGE_OPTION;
 char mediaFolder[1000];
 int stripGames;
 int shutDownEnabled;
 int selectedShutDownOption;
 int footerVisibleInFullscreenMode;
 int menuVisibleInFullscreenMode;
 int autoHideLogos;
 int timeoutValue;
 int OC_UC;
 int OC_NO;
 int OC_OC;
 int OC_SLEEP;
 int backlightValue;
 int hdmiChanged;
 pthread_t myThread;

/* THEME */
 char *themes[100];
 int activeTheme;
 int themeCounter;
 int currentMode;
 char menuFont[1000];
 int baseFont;
 int fontSize;
 int settingsFontSize;
 int transparentShading;
 int footerOnTop;
 char simpleBackground[1000];
 char fullscreenBackground[1000];
 char favoriteIndicator[1000];
 char sectionGroupsFolder[1000];

 int itemsInSimple;
 int itemsInFullSimple;
 int itemsSeparationInSimple;
 int gameListPositionInSimple;
 int gameListPositionInFullSimple;
 int headerPositionInSimple;
 int footerPositionInSimple;

 int itemsInTraditional;
 int itemsInFullTraditional;
 int itemsSeparationInTraditional;
 int gameListPositionInTraditional;
 int gameListPositionInFullTraditional;
 int headerPositionInTraditional;
 int footerPositionInTraditional;

 int itemsInDrunkenMonkey;
 int itemsInFullDrunkenMonkey;
 int itemsSeparationInDrunkenMonkey;
 int gameListPositionInDrunkenMonkey;
 int gameListPositionInFullDrunkenMonkey;
 int headerPositionInDrunkenMonkey;
 int footerPositionInDrunkenMonkey;

 int itemsInCustom;
 int itemsInFullCustom;
 int itemsSeparationInCustom;
 char textXFontCustom[1000];
 int text1FontSizeInCustom;
 int text1XInCustom;
 int text1YInCustom;
 int text1AlignmentInCustom;
 int text2FontSizeInCustom;
 int text2XInCustom;
 int text2YInCustom;
 int text2AlignmentInCustom;
 int text3FontSizeInCustom;
 int text3XInCustom;
 int text3YInCustom;
 int text3AlignmentInCustom;
 int gameListAlignmentInCustom;
 int gameListXInCustom;
 int gameListYInCustom;
 int gameListWidthInCustom;
 int gameListPositionInFullCustom;
 int artWidthInCustom;
 int artHeightInCustom;
 int artXInCustom;
 int artYInCustom;
 int artTextDistanceFromPictureInCustom;
 int artTextLineSeparationInCustom;
 int artTextFontSizeInCustom;
 int systemWidthInCustom;
 int systemHeightInCustom;
 int systemXInCustom;
 int systemYInCustom;
 int fontSizeCustom;
 int colorfulFullscreenMenu;
 int displaySectionGroupName;

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
};

 struct SectionGroup sectionGroups[100];
 int sectionGroupStates[100][100][5];
 struct MenuSection menuSections[100];
 struct Favorite favorites[2000];

/* CONTROL */
 uint8_t *keys;
 int BTN_Y;
 int BTN_B;
 int BTN_A;
 int BTN_X;
 int BTN_START;
 int BTN_SELECT;
 int BTN_R;
 int BTN_UP;
 int BTN_DOWN;
 int BTN_LEFT;
 int BTN_RIGHT;
 int BTN_L1;
 int BTN_R1;
 int BTN_L2;
 int BTN_R2;

#if defined TARGET_RG350 || defined TARGET_RG350_BETA
 Shake_Device *device;
 Shake_Effect effect;
 int effect_id;
 Shake_Effect effect1;
 int effect_id1;
#endif

/* SCREEN */
 int MAGIC_NUMBER;
 int SCREEN_HEIGHT;
 int SCREEN_WIDTH;
 double SCREEN_RATIO;
 int hdmiEnabled;

/* MISC */
 time_t currRawtime;
 struct tm * currTime;
 int lastSec;
 int lastMin;
 int lastChargeLevel;
 pthread_t clockThread;
 pthread_mutex_t lock;

#endif
