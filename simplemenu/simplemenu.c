#include <unistd.h>
#include <constants.h>
#include <definitions.h>
#include <dirent.h>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      /* needed to use pid_t, etc. */
#include <sys/wait.h>       /* needed to use wait() */
#include <string_utils.h>
#include <SDL/SDL.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>

int currentGame = 0;
int currentPage = 0;
int currentEmulator = 0;
int gamesInPage=0;
int totalPages=0;
int running = 1;
char *gameList[200][10];
static char buf[1024];

uint8_t *keys;

TTF_Font *font = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;
SDL_Color headerAndFooterBackgroundColor = DARKEST_GREEN;
SDL_Color headerFontColor = LIGHTEST_GREEN;
SDL_Color regularFontColor = DARKEST_GREEN;
SDL_Color shadedFontForegroundColor = LIGHTEST_GREEN;
SDL_Color shadedFontBackgroundColor = DARKEST_GREEN;
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
	int rgbColor[] = DARKEST_GREEN;
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
	draw_text(screen, headerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(23), consoles[currentEmulator], headerFontColor, VAlignTop | HAlignCenter);
}

void drawGameList() {
	int rgbColor[] = LIGHTEST_GREEN;
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
				draw_shaded_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, shadedFontForegroundColor, VAlignBottom | HAlignCenter, shadedFontBackgroundColor);
			} else {
				draw_text(screen, font, SCREEN_WIDTH/2, nextLine, buf, regularFontColor, VAlignBottom | HAlignCenter);
			}
			nextLine+=calculateProportionalSizeOrDistance(19);
		}
	}
}

void drawFooter() {
	int rgbColor[] = DARKEST_GREEN;
	draw_rectangle(screen, SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	draw_text(screen, font, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), "SELECT+START: SHUT DOWN", headerFontColor, VAlignMiddle | HAlignCenter);
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
		return emulatorExecutables[1];
	}
	if (strcmp(ext,".gbc\"")==0) {
		return emulatorExecutables[2];
	}
	if (strcmp(ext,".gba\"")==0) {
		return emulatorExecutables[3];
	}
	if (strcmp(ext,".nes\"")==0) {
		return emulatorExecutables[4];
	}
	if (strcmp(ext,".sfc\"")==0) {
		return emulatorExecutables[5];
	}
	if (strcmp(ext,".gg\"")==0) {
		return emulatorExecutables[6];
	}
	if (strcmp(ext,".sms\"")==0) {
		return emulatorExecutables[7];
	}
	if (strcmp(ext,".md\"")==0) {
		return emulatorExecutables[8];
	}
	if (strcmp(ext,".iso\"")==0) {
		return emulatorExecutables[9];
	}
	if (strcmp(ext,".zip\"")==0) {
		return emulatorExecutables[10];
	}
	if (strcmp(ext,".pce\"")==0) {
		return emulatorExecutables[11];
	}
	if (strcmp(ext,".bin\"")==0) {
		return emulatorExecutables[12];
	}
	return NULL;
}

void executeFavorite (char fileToBeExecutedWithFullPath[]) {
	freeResources();
	screen=NULL;
	char command[200];
	char executable[200];
	strcpy(executable,determineExecutable(fileToBeExecutedWithFullPath));
	printf("%s\n",executable);
	strcpy(command, executable);
	if(executable!=NULL) {
		strcat(command," ");
	}
	printf("%s\n",command);
	strcat(command,fileToBeExecutedWithFullPath);
	printf("%s\n",command);
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
	int n=scandir(romsDirectories[currentEmulator], &files, 0, alphasort);
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
			if(emulatorExecutables[currentEmulator+1]) {
				currentEmulator++;
				drawHeader();
				totalPages=0;
				loadGameList();
				currentGame=0;
				currentPage=0;
				return;
			}
		}
		if(keys[BTN_A] && keys[BTN_LEFT]) {
			if(currentEmulator>0) {
				currentEmulator--;
				drawHeader();
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
				strcat(fileToBeExecutedwithFullPath,romsDirectories[currentEmulator]);
				strcat(fileToBeExecutedwithFullPath,gameList[currentPage][currentGame]);
				strcat(fileToBeExecutedwithFullPath,"\"");
				strcat(fileToBeExecutedwithFullPath,"\0");
				if (currentEmulator>0) {
					executeCommand(emulatorExecutables[currentEmulator],fileToBeExecutedwithFullPath);
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

int main(int argc, char *argv[]) {
	setupDisplay();
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
