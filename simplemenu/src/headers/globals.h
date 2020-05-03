#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

#include <pthread.h>
#include <SDL/SDL_timer.h>
#include "../headers/hashtable.h"
#include "../headers/constants.h"

#ifdef TARGET_RG350
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
SDL_TimerID timeoutTimer;
SDL_TimerID picModeHideMenuTimer;
SDL_TimerID picModeHideLogoTimer;
SDL_TimerID hideHeartTimer;

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
int FONT_SIZE_OPTION;
int ITEMS_PER_PAGE_OPTION;
char mediaFolder[1000];
int stripGames;
int shutDownEnabled;
int wannaReset;
int footerVisibleInFullscreenMode;
int menuVisibleInFullscreenMode;
int autoHideLogos;
int timeoutValue;
int OC_UC;
int OC_NO;
int OC_OC;
int OC_SLEEP;
int backlightValue;

/* THEME */
char *themes[100];
int activeTheme;
int themeCounter;
int currentMode;
char menuFont[1000];
int baseFont;
int fontSize;
int footerOnTop;
char simpleBackground[1000];
char fullscreenBackground[1000];
char favoriteIndicator[1000];

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
int gameListAlignmentInCustom;
int gameListXInCustom;
int gameListYInCustom;
int gameListWidthInCustom;
int gameListPositionInFullCustom;
int artWidthInCustom;
int artHeightInCustom;
int artXInCustom;
int artYInCustom;
int systemWidthInCustom;
int systemHeightInCustom;
int systemXInCustom;
int systemYInCustom;
int fontSizeCustom;

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
};

struct MenuSection {
	char sectionName[25];
	char *emulatorDirectories[10];
	char *executables[10];
	char filesDirectories[400];
	char fileExtensions[50];
	char systemLogo[300];
	char systemPicture[300];
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
	char mask[1000];
//	char *fileList[50000];
};

struct SectionGroup sectionGroups[100];
int sectionGroupStates[100][100][5];
struct MenuSection menuSections[100];
struct Favorite favorites[2000];

/* CONTROL */
uint8_t *keys;
//SDL_Joystick *joystick;
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

#ifdef TARGET_RG350
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

/* MISC */
time_t currRawtime;
struct tm * currTime;
int lastSec;
int lastChargeLevel;
pthread_t clockThread;
pthread_mutex_t lock;

#endif
