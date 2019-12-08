#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/system_logic.h"

void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	CURRENT_SECTION.totalPages=0;
	favoritesSectionNumber=0;
	favoritesSize=0;
	currentCPU=OC_NO;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	pictureMode=0;
#ifndef TARGET_PC
#endif
	srand(time(0));
}

void resetFrameBuffer () {
	int ret = system("./frontend_start");
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
}

void sig_term_handler(int signum)
{
	running=0;
}


//void *checkClock(void *x_void_ptr) {
//	while (1) {
//		currRawtime = time (NULL);
//		currTime = localtime(&currRawtime);
//		if(currTime->tm_sec>lastSec) {
//			lastSec=currTime->tm_sec;
//			if(!isSuspended&&!leftOrRightPressed&&!isUSBMode) {
//				printf("iupdate last\n");
//				lastChargeLevel = getBatteryLevel();
//				updateScreen();
//			}
//		}
//		free (currTime);
//	}
//	printf("pepe\n");
//}

uint32_t hex2int(char *hex) {
	char *hexCopy = malloc(strlen(hex)+1);
	strcpy(hexCopy,hex);
	uint32_t val = 0;
	while (*hexCopy) {
		// get current character then increment
		uint8_t byte = *hexCopy++;
		// transform hex character to the 4bit equivalent number, using the ascii table indexes
		if (byte >= '0' && byte <= '9') byte = byte - '0';
		else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
		else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
		// shift 4 to make space for new digit, and add the 4 bits of the new digit
		val = (val << 4) | (byte & 0xF);
	}
	return val;
}

int testSectionLoad() {
	FILE * fp;
	char line[1500];
	char *configurations[26];
	char pathToSectionsFilePlusFileName[300];
	snprintf(pathToSectionsFilePlusFileName,sizeof(pathToSectionsFilePlusFileName),"%s/.simplemenu/sectionsTest.cfg",getenv("HOME"));
	fp = fopen(pathToSectionsFilePlusFileName, "r");
	if (fp==NULL) {
		return -1;
	}
	char lineNumber=0;
	struct MenuSection aMenuSection;
	char r[3];
	char g[3];
	char b[3];
	while (fgets(line, sizeof(line), fp) != NULL) {
		int dirCounter=0;
		int execCounter=0;
		if(line[0]=='#'||strlen(line)<2) {
			continue;
		}
		line[strlen(line)-1]='\0';
		char *value = strtok(line, "=");
		value = strtok(NULL, "=");
		if (value!=NULL) {
			r[0]=value[0];
			r[1]=value[1];
			r[2]='\0';
			g[0]=value[2];
			g[1]=value[3];
			g[2]='\0';
			b[0]=value[4];
			b[1]=value[5];
			b[2]='\0';
		}
		switch (lineNumber) {
		case 0:
			strcpy(aMenuSection.sectionName,value);
			break;
		case 1:
			for (int i=0;i<10;i++) {
				aMenuSection.emulatorDirectories[dirCounter]=NULL;
			}
			dirCounter=0;
			char* currentDir = strtok(value,",");
			while(currentDir!=NULL) {
				aMenuSection.emulatorDirectories[dirCounter]=malloc(strlen(currentDir)+1);
				strcpy(aMenuSection.emulatorDirectories[dirCounter],currentDir);
				strcat(aMenuSection.emulatorDirectories[dirCounter],"\0");
				currentDir = strtok(NULL,",");
				dirCounter++;
			}
			free (currentDir);
			if(dirCounter>0) {
				aMenuSection.emulatorDirectories[dirCounter]=NULL;
			}
			aMenuSection.activeEmulatorDirectory=0;
			break;
		case 2:
			for (int i=0;i<10;i++) {
				aMenuSection.executables[execCounter]=NULL;
			}
			execCounter=0;
			char* currentExec = strtok(value,",");
			while(currentExec!=NULL) {
				aMenuSection.executables[execCounter]=malloc(strlen(currentExec)+1);
				strcpy(aMenuSection.executables[execCounter],currentExec);
				currentExec = strtok(NULL,",");
				execCounter++;
			}
			free (currentExec);
			aMenuSection.executables[execCounter]=NULL;
			aMenuSection.activeExecutable=0;
			break;
		case 3:
			strcpy(aMenuSection.filesDirectories,value);
			break;
		case 4:
			strcpy(aMenuSection.fileExtensions,value);
			break;
		case 5:
			aMenuSection.headerAndFooterBackgroundColor.r=hex2int(r);
			aMenuSection.headerAndFooterBackgroundColor.g=hex2int(g);
			aMenuSection.headerAndFooterBackgroundColor.b=hex2int(b);
			break;
		case 6:
			aMenuSection.headerAndFooterTextColor.r=hex2int(r);
			aMenuSection.headerAndFooterTextColor.g=hex2int(g);
			aMenuSection.headerAndFooterTextColor.b=hex2int(b);
			break;
		case 7:
			aMenuSection.bodyBackgroundColor.r=hex2int(r);
			aMenuSection.bodyBackgroundColor.g=hex2int(g);
			aMenuSection.bodyBackgroundColor.b=hex2int(b);
			break;
		case 8:
			aMenuSection.bodyTextColor.r=hex2int(r);
			aMenuSection.bodyTextColor.g=hex2int(g);
			aMenuSection.bodyTextColor.b=hex2int(b);
			break;
		case 9:
			aMenuSection.bodySelectedTextBackgroundColor.r=hex2int(r);
			aMenuSection.bodySelectedTextBackgroundColor.g=hex2int(g);
			aMenuSection.bodySelectedTextBackgroundColor.b=hex2int(b);
			break;
		case 10:
			aMenuSection.bodySelectedTextTextColor.r=hex2int(r);
			aMenuSection.bodySelectedTextTextColor.g=hex2int(g);
			aMenuSection.bodySelectedTextTextColor.b=hex2int(b);
			break;
		case 11:
			strcpy(aMenuSection.consolePicture,value);
			break;
		case 12:
			if(value!=NULL) {
				strcpy(aMenuSection.aliasFileName,value);
			}
			break;
		case 13:
			strcpy(aMenuSection.category,value);
			break;
		case 14:
			if(strcmp("yes",value)==0) {
				aMenuSection.onlyFileNamesNoExtension=1;
			}
			aMenuSection.hidden=0;
			aMenuSection.currentPage=0;
			aMenuSection.currentGame=0;
			menuSections[menuSectionCounter]=aMenuSection;
			menuSectionCounter++;
			lineNumber=0;
			continue;
		}
		lineNumber++;
	}
	strcpy(aMenuSection.emulatorDirectories,"/some/folder/");
	strcpy(aMenuSection.executables,"favs");
	strcpy(aMenuSection.filesDirectories,"/some/folder");
	strcpy(aMenuSection.fileExtensions,".zzz");
	aMenuSection.headerAndFooterBackgroundColor.r=27;
	aMenuSection.headerAndFooterBackgroundColor.g=31;
	aMenuSection.headerAndFooterBackgroundColor.b=34;
	aMenuSection.headerAndFooterTextColor.r=231;
	aMenuSection.headerAndFooterTextColor.g=109;
	aMenuSection.headerAndFooterTextColor.b=62;
	aMenuSection.bodyBackgroundColor.r=49;
	aMenuSection.bodyBackgroundColor.g=58;
	aMenuSection.bodyBackgroundColor.b=66;
	aMenuSection.bodyTextColor.r=152;
	aMenuSection.bodyTextColor.g=157;
	aMenuSection.bodyTextColor.b=152;
	aMenuSection.bodySelectedTextBackgroundColor.r=248;
	aMenuSection.bodySelectedTextBackgroundColor.g=85;
	aMenuSection.bodySelectedTextBackgroundColor.b=41;
	aMenuSection.bodySelectedTextTextColor.r=27;
	aMenuSection.bodySelectedTextTextColor.g=33;
	aMenuSection.bodySelectedTextTextColor.b=42;
	strcpy(aMenuSection.category, "all");
	aMenuSection.onlyFileNamesNoExtension=0;
	menuSections[menuSectionCounter]=aMenuSection;
	favoritesSectionNumber=menuSectionCounter;
	fclose(fp);
	return menuSectionCounter;
}

int main(int argc, char* argv[]) {
	//	testSectionLoad();
	//	exit(0);
	//	lastChargeLevel = getBatteryLevel();
	//	currRawtime = time(NULL);
	//	currTime = localtime(&currRawtime);
	//	lastSec=currTime->tm_sec;
	//	pthread_mutex_init(&lock, NULL);
	//	pthread_create(&clockThread, NULL, checkClock,NULL);
	signal(SIGTERM, &sig_term_handler);
	#ifndef TARGET_PC
	resetFrameBuffer();
	HW_Init();
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	initializeGlobals();
	setupDisplayAndKeys();
	//	int sectionCount=loadSections();
	int sectionCount=testSectionLoad();
	loadFavorites();
	if (argv[1]!=NULL) {
		setSectionsState(argv[1]);
		currentSectionNumber=atoi(argv[2]);
		returnTo=atoi(argv[3]);
		pictureMode=atoi(argv[4]);
	} else {
		loadLastState();
	}
#ifndef TARGET_PC
	initSuspendTimer();
#endif
	determineStartingScreen(sectionCount);
	updateScreen();
	enableKeyRepeat();
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				if (!isSuspended) {
					if (!currentlyChoosingEmulator) {
						performAction();
					} else {
						performChoosingAction();
					}
				}
#ifndef TARGET_PC
				resetTimeoutTimer();
#endif
				updateScreen();
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_B) {
					hotKeyPressed=0;
					currentlySectionSwitching=0;
					updateScreen();
				}
				if(getPressedKey()==BTN_SELECT) {
					if (stripGames) {
						stripGames=0;
					} else {
						stripGames=1;
					}
					updateScreen();
				}
			}
		}
	}
	quit();
}
