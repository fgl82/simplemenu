#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED

#include <pthread.h>
#include <SDL/SDL_timer.h>
#include "../headers/hashtable.h"
#include "../headers/constants.h"

#ifdef TARGET_RG350
#include <shake.h>
#endif

int running;

int MAX_GAMES_IN_SECTION;

char mediaFolder[1000];
char simpleBackground[1000];
char fullscreenBackground[1000];
char menuFont[1000];

int itsStoppedBecauseOfAnError;
char *errorMessage;
int thereIsACriticalError;

int favoritesSectionNumber;
int favoritesSectionSelected;
int favoritesChanged;
int favoritesSize;
int returnTo;

int currentSectionNumber;
int menuSectionCounter;
int gamesInPage;

int currentCPU;
int fullscreenMode;
int hotKeyPressed;
int currentlySectionSwitching;
int currentlyChoosingEmulator;
int stripGames;
int shutDownEnabled;
int footerVisibleInFullscreenMode;
int menuVisibleInFullscreenMode;
int loading;

int OC_UC;
int OC_NO;
int OC_OC;
int OC_SLEEP;

SDL_TimerID timeoutTimer;
SDL_TimerID picModeHideMenuTimer;
SDL_TimerID picModeHideLogoTimer;
int timeoutValue;
int isPicModeMenuHidden;
int isSuspended;
int isUSBMode;
int backlightValue;

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

struct MenuSection {
	char sectionName[25];
	char *emulatorDirectories[10];
	char *executables[10];
	char filesDirectories[400];
	char fileExtensions[50];
	char consolePicture[100];
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
};


struct MenuSection menuSections[100];

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

struct Favorite favorites[2000];

uint8_t *keys;
//SDL_Joystick *joystick;

#ifdef TARGET_RG350
Shake_Device *device;
Shake_Effect effect;
int effect_id;
#endif

time_t currRawtime;
struct tm * currTime;
int lastSec;
int lastChargeLevel;
pthread_t clockThread;
pthread_mutex_t lock;

#endif
