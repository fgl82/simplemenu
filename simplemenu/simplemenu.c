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
int gamesInPage=0;
int totalPages=0;
int running = 1;
char * gameList[200][10];
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
	draw_text(screen, headerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(23), "999 IN 1", headerFontColor, VAlignTop | HAlignCenter);
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
	//	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 155, 188, 15));
	drawHeader();
	drawFooter();
}

void executeRom (char *rom) {
	freeResources();
	char command[200];
	strcpy(command, emulatorExecutable);
	strcat(command," ");
	strcat(command,rom);
	system(command);
	setupDisplay();
	setupDecorations();
}

void loadGameList() {
	struct dirent **namelist;
	int n=scandir(romsDirectory, &namelist, 0, alphasort);
	if (n==-1) {
		n = scandir(NESromsDirectory, &namelist, 0, alphasort);
	}
	int game = 0;
	int page = 0;
	for (int i=0;i<n;i++){
		if (strcmp((namelist[i]->d_name),"..")!=0 && strcmp((namelist[i]->d_name),".")!=0){
			gameList[page][game] = namelist[i]->d_name;
			game++;
			if (game==ITEMS_PER_PAGE) {
				page++;
				totalPages++;
				game = 0;
			}
		}
	}
	free(namelist);
}

void updateScreen() {
	drawGameList();
	SDL_Flip(screen);
}

void performAction(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		if (keys[BTN_SELECT] && keys[BTN_START]) {
			running=0;
			return;
		} else if (keys[BTN_B]) {
			char romToBeExecuted[100];
			strcpy(romToBeExecuted,"\"");
			strcat(romToBeExecuted,romsDirectory);
			strcat(romToBeExecuted,gameList[currentPage][currentGame]);
			strcat(romToBeExecuted,"\"");
			strcat(romToBeExecuted,"\0");
			executeRom(romToBeExecuted);
			return;
		} else if (keys[BTN_DOWN]) {
			if (currentGame < gamesInPage-1) {
				currentGame++;
			}
			return;
		} else if(keys[BTN_UP]) {
			if (currentGame>0) {
				currentGame--;
			}
			return;
		} else if(keys[BTN_RIGHT]) {
			if (currentPage < totalPages) {
				currentGame=0;
				currentPage++;
			}
			return;
		} else if(keys[BTN_LEFT]) {
			if (currentPage > 0) {
				currentGame=0;
				currentPage--;
			}
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	setupDisplay();
	setupDecorations();
	loadGameList();
	while (running) {
		updateScreen();
		SDL_WaitEvent(&event);
		performAction(event);
	}
	quit();
}
