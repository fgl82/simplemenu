#include <unistd.h>
#include <constants.h>
#include <definitions.h>
#include <dirent.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_utils.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

int currentGame = 0;
int currentPage = 0;
int currentSection = 0;
int gamesInPage=0;
int totalPages=0;
int running = 1;
char *gameList[200][10];
static char buf[1024];

struct MenuSection {
	char sectionName[50];
	char executable[100];
	char filesDirectory[100];
	SDL_Color headerAndFooterTextBackgroundColor;
	SDL_Color headerAndFooterTextForegroundColor;
	SDL_Color bodyTextForegroundColor;
	SDL_Color bodySelectedTextBackgroundColor;
	SDL_Color bodySelectedTextForegroundColor;
	SDL_Color bodyBackgroundColor;
};

struct MenuSection menuSections[12];

uint8_t *keys;

TTF_Font *font = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;
SDL_Surface *screen = NULL;
SDL_Event event;

int calculateProportionalSizeOrDistance(int number) {
	return (SCREEN_HEIGHT*number)/240;
}

void freeResources() {
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	SDL_Quit();
	TTF_Quit();
}

void quit() {
	freeResources();
	execlp("sh", "sh", "-c", "kill $(ps -al | grep \"/mnt/\" | grep -v \"/kernel/\" | tr -s [:blank:] | cut -d \" \" -f 2) ; sleep 0.1 ; sync && poweroff",  NULL);
}

void drawHeader() {
	int rgbColor[] = {menuSections[currentSection].headerAndFooterTextBackgroundColor.r,menuSections[currentSection].headerAndFooterTextBackgroundColor.g,menuSections[currentSection].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
	draw_text(screen, headerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(23), menuSections[currentSection].sectionName, menuSections[currentSection].headerAndFooterTextForegroundColor, VAlignTop | HAlignCenter);
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSection].bodyBackgroundColor.r,menuSections[currentSection].bodyBackgroundColor.g,menuSections[currentSection].bodyBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor);
	int nextLine = calculateProportionalSizeOrDistance(29);
	gamesInPage=0;
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (gameList[currentPage][i]!=NULL) {
			gamesInPage++;
			char *nameWithoutExtension = removeExtension(gameList[currentPage][i]);
			int charNumber = 0;
			while (nameWithoutExtension[charNumber]) {
				if (nameWithoutExtension[charNumber]=='('||charNumber>37) {
					nameWithoutExtension[charNumber]='\0';
					break;
				}
				charNumber++;
			}
			sprintf(buf,"%s", nameWithoutExtension);
			free(nameWithoutExtension);
			if (i==currentGame) {
				draw_shaded_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSection].bodySelectedTextForegroundColor, VAlignBottom | HAlignCenter, menuSections[currentSection].bodySelectedTextBackgroundColor);
			} else {
				draw_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, menuSections[currentSection].bodyTextForegroundColor, VAlignBottom | HAlignCenter);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void drawFooter() {
	int rgbColor[] = {menuSections[currentSection].headerAndFooterTextBackgroundColor.r,menuSections[currentSection].headerAndFooterTextBackgroundColor.g,menuSections[currentSection].headerAndFooterTextBackgroundColor.b};
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	draw_text(screen, font, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), "SELECT+START: SHUT DOWN", menuSections[currentSection].headerAndFooterTextForegroundColor, VAlignMiddle | HAlignCenter);
}

void setupDisplay() {
	keys = SDL_GetKeyState(NULL);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_PumpEvents();
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
	TTF_Init();
	font = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(14));
	headerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("akashi.ttf", calculateProportionalSizeOrDistance(16));
}

void setupDecorations() {
	drawHeader();
	drawFooter();
}

char *getExt (char *stringWithExtension) {
    return strrchr(stringWithExtension, '.');;
}

char *determineExecutable(char *fileToBeExecutedWithFullPath) {
	char *ext = getExt(fileToBeExecutedWithFullPath);
	if (strcmp(ext,".gb\"")==0) {
		return menuSections[1].executable;
	}
	if (strcmp(ext,".gbc\"")==0) {
		return menuSections[2].executable;
	}
	if (strcmp(ext,".gba\"")==0) {
		return menuSections[3].executable;
	}
	if (strcmp(ext,".nes\"")==0) {
		return menuSections[4].executable;
	}
	if (strcmp(ext,".sfc\"")==0) {
		return menuSections[5].executable;
	}
	if (strcmp(ext,".gg\"")==0) {
		return menuSections[6].executable;
	}
	if (strcmp(ext,".sms\"")==0) {
		return menuSections[7].executable;
	}
	if (strcmp(ext,".md\"")==0) {
		return menuSections[8].executable;
	}
	if (strcmp(ext,".iso\"")==0) {
		return menuSections[9].executable;
	}
	if (strcmp(ext,".zip\"")==0) {
		return menuSections[10].executable;
	}
	if (strcmp(ext,".pce\"")==0) {
		return menuSections[11].executable;
	}
	if (strcmp(ext,".bin\"")==0) {
		return menuSections[12].executable;
	}
	return NULL;
}

void executeFavorite (char fileToBeExecutedWithFullPath[]) {
	freeResources();
	screen=NULL;
	char command[200];
	char executable[200];
	strcpy(executable,determineExecutable(fileToBeExecutedWithFullPath));
	strcpy(command, executable);
	if(executable!=NULL) {
		strcat(command," ");
	}
	strcat(command,fileToBeExecutedWithFullPath);
	int returnValue = system(command);
	if (returnValue==-1) {
		printf("ERROR");
	}
	SDL_Init(SDL_INIT_VIDEO);
	setupDisplay();
	setupDecorations();
}

void executeCommand (char executable[], char fileToBeExecutedWithFullPath[]) {
	freeResources();
	char command[200];
	strcpy(command, executable);
	strcat(command," ");
	strcat(command,fileToBeExecutedWithFullPath);
	int returnValue = system(command);
	if (returnValue==-1) {
		printf("ERROR");
	}
	setupDisplay();
	setupDecorations();
}

void loadGameList() {
	struct dirent **files;
	int n=scandir(menuSections[currentSection].filesDirectory, &files, 0, alphasort);
	int game = 0;
	int page = 0;
	for (int i=0;i<200;i++) {
		for (int j=0;j<10;j++) {
			gameList[i][j]=NULL;
		}
	}
	for (int i=0;i<n;i++){
		if (strcmp((files[i]->d_name),".gitignore")!=0 && strcmp((files[i]->d_name),"..")!=0 && strcmp((files[i]->d_name),".")!=0){
			gameList[page][game] = files[i]->d_name;
			game++;
			if (game==ITEMS_PER_PAGE) {
				page++;
				totalPages++;
				game = 0;
			}
		}
	}
	free(files);
}

void updateScreen() {
	drawGameList();
	SDL_Flip(screen);
}

void performAction(SDL_Event event) {
		if (keys[BTN_SELECT] && keys[BTN_START]) {
			running=0;
			return;
		}
		if (keys[BTN_A] && keys[BTN_START]) {
			freeResources();
			exit(0);
		}
		if(keys[BTN_A] && keys[BTN_RIGHT]) {
			if(currentSection<(sizeof menuSections/sizeof menuSections[0])-1) {
				currentSection++;
				setupDecorations();
				totalPages=0;
				loadGameList();
				currentGame=0;
				currentPage=0;
				return;
			}
		}
		if(keys[BTN_A] && keys[BTN_LEFT]) {
			if(currentSection>0) {
				currentSection--;
				setupDecorations();
				totalPages=0;
				loadGameList();
				currentGame=0;
				currentPage=0;
				return;
			}
		}
		if (keys[BTN_B]) {
			char fileToBeExecutedwithFullPath[200];
			if (gameList[currentPage][currentGame]!=NULL) {
				strcpy(fileToBeExecutedwithFullPath,"\"");
				strcat(fileToBeExecutedwithFullPath,menuSections[currentSection].filesDirectory);
				strcat(fileToBeExecutedwithFullPath,gameList[currentPage][currentGame]);
				strcat(fileToBeExecutedwithFullPath,"\"");
				strcat(fileToBeExecutedwithFullPath,"\0");
				if (currentSection>0) {
					executeCommand(menuSections[currentSection].executable,fileToBeExecutedwithFullPath);
				} else {
					executeFavorite(fileToBeExecutedwithFullPath);
				}
			}
			return;
		}
		if (keys[BTN_DOWN]) {
			if(currentGame == gamesInPage-1) {
				if (currentPage < totalPages) {
					currentGame=0;
					currentPage++;
					return;
				}
			}
			if (currentGame < gamesInPage-1) {
				currentGame++;
				return;
			}
		}
		if(keys[BTN_UP]) {
			if(currentGame == 0) {
				if (currentPage>0) {
					currentGame=gamesInPage-1;
					currentPage--;
					return;
				}
			}
			if (currentGame > 0) {
				currentGame--;
				return;
			}
		}
		if(keys[BTN_RIGHT]) {
			if (currentPage < totalPages) {
				currentGame=0;
				currentPage++;
			}
		}
		if(keys[BTN_LEFT]) {
			if (currentPage > 0) {
				currentGame=0;
				currentPage--;
			}
			return;
		}

}

void setupSections() {
	struct MenuSection aMenuSection;

	strcpy(aMenuSection.sectionName,"9999 IN 1");
	strcpy(aMenuSection.executable,"");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/FAVORITES/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=0;
	aMenuSection.headerAndFooterTextBackgroundColor.g=92;
	aMenuSection.headerAndFooterTextBackgroundColor.b=9;
	aMenuSection.headerAndFooterTextForegroundColor.r=1;
	aMenuSection.headerAndFooterTextForegroundColor.g=252;
	aMenuSection.headerAndFooterTextForegroundColor.b=0;
	aMenuSection.bodyBackgroundColor.r=1;
	aMenuSection.bodyBackgroundColor.g=252;
	aMenuSection.bodyBackgroundColor.b=0;
	aMenuSection.bodyTextForegroundColor.r=0;
	aMenuSection.bodyTextForegroundColor.g=92;
	aMenuSection.bodyTextForegroundColor.b=9;
	aMenuSection.bodySelectedTextBackgroundColor.r=0;
	aMenuSection.bodySelectedTextBackgroundColor.g=92;
	aMenuSection.bodySelectedTextBackgroundColor.b=9;
	aMenuSection.bodySelectedTextForegroundColor.r=1;
	aMenuSection.bodySelectedTextForegroundColor.g=252;
	aMenuSection.bodySelectedTextForegroundColor.b=0;
	menuSections[0]=aMenuSection;

	strcpy(aMenuSection.sectionName,"GAME BOY");
	strcpy(aMenuSection.executable,"/mnt/emus/gambatte/gambatte");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/GB/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=142;
	aMenuSection.headerAndFooterTextBackgroundColor.g=138;
	aMenuSection.headerAndFooterTextBackgroundColor.b=153;
	aMenuSection.headerAndFooterTextForegroundColor.r=41;
	aMenuSection.headerAndFooterTextForegroundColor.g=39;
	aMenuSection.headerAndFooterTextForegroundColor.b=140;
	aMenuSection.bodyBackgroundColor.r=226;
	aMenuSection.bodyBackgroundColor.g=215;
	aMenuSection.bodyBackgroundColor.b=210;
	aMenuSection.bodyTextForegroundColor.r=0;
	aMenuSection.bodyTextForegroundColor.g=0;
	aMenuSection.bodyTextForegroundColor.b=0;
	aMenuSection.bodySelectedTextBackgroundColor.r=172;
	aMenuSection.bodySelectedTextBackgroundColor.g=57;
	aMenuSection.bodySelectedTextBackgroundColor.b=107;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[1]=aMenuSection;

	strcpy(aMenuSection.sectionName,"GAME BOY COLOR");
	strcpy(aMenuSection.executable,"/mnt/emus/gambatte/gambatte");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/GB/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=43;
	aMenuSection.headerAndFooterTextBackgroundColor.g=43;
	aMenuSection.headerAndFooterTextBackgroundColor.b=51;
	aMenuSection.headerAndFooterTextForegroundColor.r=255;
	aMenuSection.headerAndFooterTextForegroundColor.g=255;
	aMenuSection.headerAndFooterTextForegroundColor.b=255;
	aMenuSection.bodyBackgroundColor.r=147;
	aMenuSection.bodyBackgroundColor.g=198;
	aMenuSection.bodyBackgroundColor.b=36;
	aMenuSection.bodyTextForegroundColor.r=48;
	aMenuSection.bodyTextForegroundColor.g=48;
	aMenuSection.bodyTextForegroundColor.b=49;
	aMenuSection.bodySelectedTextBackgroundColor.r=66;
	aMenuSection.bodySelectedTextBackgroundColor.g=66;
	aMenuSection.bodySelectedTextBackgroundColor.b=124;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[2]=aMenuSection;

	strcpy(aMenuSection.sectionName,"GAME BOY ADVANCE");
	strcpy(aMenuSection.executable,"/mnt/emus/gpsp/gpsp");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/GBA/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=21;
	aMenuSection.headerAndFooterTextBackgroundColor.g=18;
	aMenuSection.headerAndFooterTextBackgroundColor.b=26;
	aMenuSection.headerAndFooterTextForegroundColor.r=255;
	aMenuSection.headerAndFooterTextForegroundColor.g=255;
	aMenuSection.headerAndFooterTextForegroundColor.b=255;
	aMenuSection.bodyBackgroundColor.r=82;
	aMenuSection.bodyBackgroundColor.g=67;
	aMenuSection.bodyBackgroundColor.b=134;
	aMenuSection.bodyTextForegroundColor.r=0;
	aMenuSection.bodyTextForegroundColor.g=0;
	aMenuSection.bodyTextForegroundColor.b=0;
	aMenuSection.bodySelectedTextBackgroundColor.r=81;
	aMenuSection.bodySelectedTextBackgroundColor.g=188;
	aMenuSection.bodySelectedTextBackgroundColor.b=231;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[3]=aMenuSection;

	strcpy(aMenuSection.sectionName,"NES");
	strcpy(aMenuSection.executable,"/mnt/emus/fceux/fceux");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/NES/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=59;
	aMenuSection.headerAndFooterTextBackgroundColor.g=59;
	aMenuSection.headerAndFooterTextBackgroundColor.b=59;
	aMenuSection.headerAndFooterTextForegroundColor.r=184;
	aMenuSection.headerAndFooterTextForegroundColor.g=62;
	aMenuSection.headerAndFooterTextForegroundColor.b=57;
	aMenuSection.bodyBackgroundColor.r=203;
	aMenuSection.bodyBackgroundColor.g=200;
	aMenuSection.bodyBackgroundColor.b=197;
	aMenuSection.bodyTextForegroundColor.r=0;
	aMenuSection.bodyTextForegroundColor.g=0;
	aMenuSection.bodyTextForegroundColor.b=0;
	aMenuSection.bodySelectedTextBackgroundColor.r=200;
	aMenuSection.bodySelectedTextBackgroundColor.g=0;
	aMenuSection.bodySelectedTextBackgroundColor.b=0;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[4]=aMenuSection;

	strcpy(aMenuSection.sectionName,"SNES");
	strcpy(aMenuSection.executable,"/mnt/emus/snes9x4d/snes9x4d");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/SNES/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=130;
	aMenuSection.headerAndFooterTextBackgroundColor.g=95;
	aMenuSection.headerAndFooterTextBackgroundColor.b=184;
	aMenuSection.headerAndFooterTextForegroundColor.r=202;
	aMenuSection.headerAndFooterTextForegroundColor.g=191;
	aMenuSection.headerAndFooterTextForegroundColor.b=255;
	aMenuSection.bodyBackgroundColor.r=205;
	aMenuSection.bodyBackgroundColor.g=205;
	aMenuSection.bodyBackgroundColor.b=207;
	aMenuSection.bodyTextForegroundColor.r=72;
	aMenuSection.bodyTextForegroundColor.g=78;
	aMenuSection.bodyTextForegroundColor.b=85;
	aMenuSection.bodySelectedTextBackgroundColor.r=114;
	aMenuSection.bodySelectedTextBackgroundColor.g=113;
	aMenuSection.bodySelectedTextBackgroundColor.b=133;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[5]=aMenuSection;

	strcpy(aMenuSection.sectionName,"GAME GEAR");
	strcpy(aMenuSection.executable,"/mnt/emus/sms/sms");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/GG/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=26;
	aMenuSection.headerAndFooterTextBackgroundColor.g=100;
	aMenuSection.headerAndFooterTextBackgroundColor.b=181;
	aMenuSection.headerAndFooterTextForegroundColor.r=195;
	aMenuSection.headerAndFooterTextForegroundColor.g=195;
	aMenuSection.headerAndFooterTextForegroundColor.b=195;
	aMenuSection.bodyBackgroundColor.r=0;
	aMenuSection.bodyBackgroundColor.g=1;
	aMenuSection.bodyBackgroundColor.b=2;
	aMenuSection.bodyTextForegroundColor.r=195;
	aMenuSection.bodyTextForegroundColor.g=195;
	aMenuSection.bodyTextForegroundColor.b=195;
	aMenuSection.bodySelectedTextBackgroundColor.r=96;
	aMenuSection.bodySelectedTextBackgroundColor.g=98;
	aMenuSection.bodySelectedTextBackgroundColor.b=102;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[6]=aMenuSection;

	strcpy(aMenuSection.sectionName,"MASTER SYSTEM");
	strcpy(aMenuSection.executable,"/mnt/emus/sms/sms");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/SMS/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=118;
	aMenuSection.headerAndFooterTextBackgroundColor.g=26;
	aMenuSection.headerAndFooterTextBackgroundColor.b=27;
	aMenuSection.headerAndFooterTextForegroundColor.r=247;
	aMenuSection.headerAndFooterTextForegroundColor.g=237;
	aMenuSection.headerAndFooterTextForegroundColor.b=246;
	aMenuSection.bodyBackgroundColor.r=31;
	aMenuSection.bodyBackgroundColor.g=32;
	aMenuSection.bodyBackgroundColor.b=34;
	aMenuSection.bodyTextForegroundColor.r=246;
	aMenuSection.bodyTextForegroundColor.g=237;
	aMenuSection.bodyTextForegroundColor.b=247;
	aMenuSection.bodySelectedTextBackgroundColor.r=192;
	aMenuSection.bodySelectedTextBackgroundColor.g=36;
	aMenuSection.bodySelectedTextBackgroundColor.b=33;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[7]=aMenuSection;

	strcpy(aMenuSection.sectionName,"SEGA GENESIS");
	strcpy(aMenuSection.executable,"/mnt/emus/picodrive/picodrive");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/SMD/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=172;
	aMenuSection.headerAndFooterTextBackgroundColor.g=181;
	aMenuSection.headerAndFooterTextBackgroundColor.b=180;
	aMenuSection.headerAndFooterTextForegroundColor.r=12;
	aMenuSection.headerAndFooterTextForegroundColor.g=8;
	aMenuSection.headerAndFooterTextForegroundColor.b=9;
	aMenuSection.bodyBackgroundColor.r=13;
	aMenuSection.bodyBackgroundColor.g=13;
	aMenuSection.bodyBackgroundColor.b=15;
	aMenuSection.bodyTextForegroundColor.r=246;
	aMenuSection.bodyTextForegroundColor.g=237;
	aMenuSection.bodyTextForegroundColor.b=247;
	aMenuSection.bodySelectedTextBackgroundColor.r=120;
	aMenuSection.bodySelectedTextBackgroundColor.g=0;
	aMenuSection.bodySelectedTextBackgroundColor.b=2;
	aMenuSection.bodySelectedTextForegroundColor.r=255;
	aMenuSection.bodySelectedTextForegroundColor.g=255;
	aMenuSection.bodySelectedTextForegroundColor.b=255;
	menuSections[8]=aMenuSection;

	strcpy(aMenuSection.sectionName,"PLAYSTATION");
	strcpy(aMenuSection.executable,"/mnt/emus/pcsx_rearmed/pcsx");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/PS1/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=190;
	aMenuSection.headerAndFooterTextBackgroundColor.g=187;
	aMenuSection.headerAndFooterTextBackgroundColor.b=191;
	aMenuSection.headerAndFooterTextForegroundColor.r=84;
	aMenuSection.headerAndFooterTextForegroundColor.g=86;
	aMenuSection.headerAndFooterTextForegroundColor.b=88;
	aMenuSection.bodyBackgroundColor.r=113;
	aMenuSection.bodyBackgroundColor.g=115;
	aMenuSection.bodyBackgroundColor.b=112;
	aMenuSection.bodyTextForegroundColor.r=62;
	aMenuSection.bodyTextForegroundColor.g=189;
	aMenuSection.bodyTextForegroundColor.b=193;
	aMenuSection.bodySelectedTextBackgroundColor.r=184;
	aMenuSection.bodySelectedTextBackgroundColor.g=104;
	aMenuSection.bodySelectedTextBackgroundColor.b=113;
	aMenuSection.bodySelectedTextForegroundColor.r=135;
	aMenuSection.bodySelectedTextForegroundColor.g=195;
	aMenuSection.bodySelectedTextForegroundColor.b=241;
	menuSections[9]=aMenuSection;

	strcpy(aMenuSection.sectionName,"NEO-GEO");
	strcpy(aMenuSection.executable,"/mnt/emus/gngeo/gngeo");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/NEOGEO/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=67;
	aMenuSection.headerAndFooterTextBackgroundColor.g=67;
	aMenuSection.headerAndFooterTextBackgroundColor.b=67;
	aMenuSection.headerAndFooterTextForegroundColor.r=230;
	aMenuSection.headerAndFooterTextForegroundColor.g=197;
	aMenuSection.headerAndFooterTextForegroundColor.b=137;
	aMenuSection.bodyBackgroundColor.r=57;
	aMenuSection.bodyBackgroundColor.g=57;
	aMenuSection.bodyBackgroundColor.b=57;
	aMenuSection.bodyTextForegroundColor.r=213;
	aMenuSection.bodyTextForegroundColor.g=213;
	aMenuSection.bodyTextForegroundColor.b=214;
	aMenuSection.bodySelectedTextBackgroundColor.r=230;
	aMenuSection.bodySelectedTextBackgroundColor.g=197;
	aMenuSection.bodySelectedTextBackgroundColor.b=137;
	aMenuSection.bodySelectedTextForegroundColor.r=57;
	aMenuSection.bodySelectedTextForegroundColor.g=57;
	aMenuSection.bodySelectedTextForegroundColor.b=57;
	menuSections[10]=aMenuSection;

	strcpy(aMenuSection.sectionName,"PC ENGINE");
	strcpy(aMenuSection.executable,"/mnt/emus/temper/temper");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/PCE/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=225;
	aMenuSection.headerAndFooterTextBackgroundColor.g=224;
	aMenuSection.headerAndFooterTextBackgroundColor.b=219;
	aMenuSection.headerAndFooterTextForegroundColor.r=204;
	aMenuSection.headerAndFooterTextForegroundColor.g=72;
	aMenuSection.headerAndFooterTextForegroundColor.b=57;
	aMenuSection.bodyBackgroundColor.r=195;
	aMenuSection.bodyBackgroundColor.g=194;
	aMenuSection.bodyBackgroundColor.b=189;
	aMenuSection.bodyTextForegroundColor.r=0;
	aMenuSection.bodyTextForegroundColor.g=0;
	aMenuSection.bodyTextForegroundColor.b=2;
	aMenuSection.bodySelectedTextBackgroundColor.r=37;
	aMenuSection.bodySelectedTextBackgroundColor.g=175;
	aMenuSection.bodySelectedTextBackgroundColor.b=124;
	aMenuSection.bodySelectedTextForegroundColor.r=238;
	aMenuSection.bodySelectedTextForegroundColor.g=239;
	aMenuSection.bodySelectedTextForegroundColor.b=233;
	menuSections[11]=aMenuSection;

	strcpy(aMenuSection.sectionName,"ATARI");
	strcpy(aMenuSection.executable,"/mnt/emus/stella/stella");
	strcpy(aMenuSection.filesDirectory,"/mnt/roms/2600/");
	aMenuSection.headerAndFooterTextBackgroundColor.r=171;
	aMenuSection.headerAndFooterTextBackgroundColor.g=91;
	aMenuSection.headerAndFooterTextBackgroundColor.b=38;
	aMenuSection.headerAndFooterTextForegroundColor.r=241;
	aMenuSection.headerAndFooterTextForegroundColor.g=231;
	aMenuSection.headerAndFooterTextForegroundColor.b=229;
	aMenuSection.bodyBackgroundColor.r=62;
	aMenuSection.bodyBackgroundColor.g=53;
	aMenuSection.bodyBackgroundColor.b=74;
	aMenuSection.bodyTextForegroundColor.r=254;
	aMenuSection.bodyTextForegroundColor.g=205;
	aMenuSection.bodyTextForegroundColor.b=138;
	aMenuSection.bodySelectedTextBackgroundColor.r=148;
	aMenuSection.bodySelectedTextBackgroundColor.g=34;
	aMenuSection.bodySelectedTextBackgroundColor.b=37;
	aMenuSection.bodySelectedTextForegroundColor.r=254;
	aMenuSection.bodySelectedTextForegroundColor.g=205;
	aMenuSection.bodySelectedTextForegroundColor.b=138;
	menuSections[11]=aMenuSection;
}

int main(int argc, char *argv[]) {
	setupDisplay();
	setupSections();
	setupDecorations();
	loadGameList();
	updateScreen();
	while (running) {
		SDL_EnableKeyRepeat(30,180);
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				performAction(event);
				updateScreen();
			}
			if (event.type == SDL_KEYUP) {
				break;
			}
		}
	}
	quit();
}
