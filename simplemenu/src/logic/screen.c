#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <opk.h>

#include "../headers/constants.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/string_utils.h"
#include "../headers/doubly_linked_rom_list.h"
#include "../headers/utils.h"

int countDown;

void displayBackgroundPicture() {
	if(fullscreenMode) {
		drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0,0,CURRENT_SECTION.bodyBackgroundColor);
	} else {
		drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0,0,CURRENT_SECTION.bodyBackgroundColor);
	}
}


void showErrorMessage(char *errorMessage) {
	int width = ((calculateProportionalSizeOrDistance(strlen(errorMessage)))*(180)/calculateProportionalSizeOrDistance(18));
	int height = calculateProportionalSizeOrDistance(40);
	if(strchr(errorMessage,'-')!=NULL) {
		height = calculateProportionalSizeOrDistance(60);
		width = ((calculateProportionalSizeOrDistance(strlen(errorMessage))/2*calculateProportionalSizeOrDistance(200))/calculateProportionalSizeOrDistance(18))+calculateProportionalSizeOrDistance(20);
	}
	int filling[3];
	int borderColor[3];
	borderColor[0]=255;
	borderColor[1]=0;
	borderColor[2]=0;
	filling[0]=100;
	filling[1]=0;
	filling[2]=0;
	int textColor[3]={255, 255, 255};
	drawRectangleToScreen(width+calculateProportionalSizeOrDistance(10), height+calculateProportionalSizeOrDistance(10), SCREEN_WIDTH/2-width/2-calculateProportionalSizeOrDistance(5),SCREEN_HEIGHT/2-height/2-calculateProportionalSizeOrDistance(5), borderColor);
	drawRectangleToScreen(width, height, SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2-height/2, filling);
	drawError(errorMessage, textColor);
	itsStoppedBecauseOfAnError=0;
//	free(errorMessage);
}

int letterExistsInGameList(char *letter, char* letters) {
	if (strstr(letters,letter)!=NULL) {
		return 1;
	}
	return 0;
}

void showLetter(struct Rom *rom) {
	int rectangleHeight = 80;
	int rectangleX = (SCREEN_WIDTH/2);
	int rectangleY = (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3);
	int filling[3];
	filling[0]=CURRENT_SECTION.fullScreenMenuBackgroundColor[0];
	filling[1]=CURRENT_SECTION.fullScreenMenuBackgroundColor[1];
	filling[2]=CURRENT_SECTION.fullScreenMenuBackgroundColor[2];
	int textColor[3] = {CURRENT_SECTION.fullscreenMenuItemsColor[0], CURRENT_SECTION.fullscreenMenuItemsColor[1], CURRENT_SECTION.fullscreenMenuItemsColor[2]};
	if (fullscreenMode) {
		filling[0] = 0;
		filling[1] = 0;
		filling[2] = 0;
		textColor[0]=255;
		textColor[1]=255;
		textColor[2]=255;
		rectangleHeight=calculateProportionalSizeOrDistance(20);
		rectangleX = 0;
		rectangleY = calculateProportionalSizeOrDistance(220);
	}
	filling[0] = CURRENT_SECTION.fullScreenMenuBackgroundColor[0];
	filling[1] = CURRENT_SECTION.fullScreenMenuBackgroundColor[1];
	filling[2] = CURRENT_SECTION.fullScreenMenuBackgroundColor[2];
	textColor[0]=CURRENT_SECTION.fullscreenMenuItemsColor[0];
	textColor[1]=CURRENT_SECTION.fullscreenMenuItemsColor[0];
	textColor[2]=CURRENT_SECTION.fullscreenMenuItemsColor[0];
	rectangleHeight=calculateProportionalSizeOrDistance(21);
	rectangleX = 0;
	rectangleY = calculateProportionalSizeOrDistance(219);
	if (!fullscreenMode) {
		drawRectangleToScreen(SCREEN_WIDTH, rectangleHeight, rectangleX, rectangleY, filling);
		drawTransparentRectangleToScreen(SCREEN_WIDTH, rectangleHeight, rectangleX, rectangleY, (int[]){0,0,0},255);
	} else {
		drawRectangleToScreen(SCREEN_WIDTH, rectangleHeight, rectangleX, rectangleY, (int[]){0,0,0});
	}
	char currentGameFirstLetter[2]="";
	char *currentGame = malloc(500);
	currentGame=getFileNameOrAlias(rom);
	currentGameFirstLetter[0]=toupper(currentGame[0]);
	currentGameFirstLetter[1]='\0';

	char *letters[] = {"#", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
	char *existingLetters = getCurrentSectionExistingLetters();

	if(isdigit(currentGameFirstLetter[0])) {
		currentGameFirstLetter[0]='#';
	}
	int x = 0;
	int y = calculateProportionalSizeOrDistance(231);
	for (int i=0;i<27;i++) {
		if (!letterExistsInGameList(letters[i], existingLetters)) {
			textColor[0]=40;
			textColor[1]=40;
			textColor[2]=40;
		} else {
			textColor[0]=255;
			textColor[1]=255;
			textColor[2]=255;
		}
		if (strcmp(letters[i],currentGameFirstLetter)==0) {
			textColor[0]=255;
			textColor[1]=0;
			textColor[2]=0;
		}
		if (strcmp(letters[i],"N")==0) {
			x+=calculateProportionalSizeOrDistance(14);
		} else if (strcmp(letters[i],"A")==0) {
			x+=calculateProportionalSizeOrDistance(13);
		} else if (strcmp(letters[i],"B")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"C")==0) {
			x+=calculateProportionalSizeOrDistance(11);
		} else if (strcmp(letters[i],"D")==0) {
			x+=calculateProportionalSizeOrDistance(11);
		} else if (strcmp(letters[i],"F")==0) {
			x+=calculateProportionalSizeOrDistance(10);
		} else if (strcmp(letters[i],"G")==0) {
			x+=calculateProportionalSizeOrDistance(10);
		} else if (strcmp(letters[i],"H")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"I")==0) {
			x+=calculateProportionalSizeOrDistance(10);
		} else if (strcmp(letters[i],"J")==0) {
			x+=calculateProportionalSizeOrDistance(8);
		} else if (strcmp(letters[i],"K")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"M")==0) {
			x+=calculateProportionalSizeOrDistance(13);
		} else if (strcmp(letters[i],"O")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"P")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"Q")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"R")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"S")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"V")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else if (strcmp(letters[i],"W")==0) {
			x+=calculateProportionalSizeOrDistance(13);
		} else if (strcmp(letters[i],"X")==0) {
			x+=calculateProportionalSizeOrDistance(14);
		} else if (strcmp(letters[i],"Y")==0) {
			x+=calculateProportionalSizeOrDistance(11);
		} else if (strcmp(letters[i],"Z")==0) {
			x+=calculateProportionalSizeOrDistance(12);
		} else {
			x+=calculateProportionalSizeOrDistance(11);
		}
		if(!is43()&&x>calculateProportionalSizeOrDistance(14)) {
			x+=calculateProportionalSizeOrDistance(4);
		}
		drawCurrentLetter(letters[i], textColor, x, y);
	}
	free(existingLetters);
	free(currentGame);
}


void showCurrentGroup() {
	int height = SCREEN_HEIGHT;
	int backgroundColor[3];
	backgroundColor[0]=30;
	backgroundColor[1]=30;
	backgroundColor[2]=130;
	int textColor[3]= {255, 255, 255};
	char *tempString = malloc(strlen(sectionGroups[activeGroup].groupName)+1);
	strcpy(tempString,sectionGroups[activeGroup].groupName);
	strcat(tempString,"\0");
	drawRectangleToScreen(calculateProportionalSizeOrDistance(SCREEN_WIDTH), calculateProportionalSizeOrDistance(height), 0, 0, backgroundColor);
	displayCenteredSurface(sectionGroups[activeGroup].groupBackgroundSurface);
	if (displaySectionGroupName) {
		drawTransparentRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(70), 0, SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(38), backgroundColor, 50);
		drawCurrentSectionGroup(tempString, textColor);
	}
	free(tempString);
	logMessage("INFO", "Group displayed");
}

void showRomPreferences() {
	int textColor[3];
	textColor[0]=90;
	textColor[1]=90;
	textColor[2]=90;
	int valueColor[3] = {0,147,131};

	int textWidth;

	char *emuName = malloc(strlen(CURRENT_SECTION.executables[CURRENT_SECTION.currentGameNode->data->preferences.emulator])+1);
	strcpy(emuName,CURRENT_SECTION.executables[CURRENT_SECTION.currentGameNode->data->preferences.emulator]);
	strcat(emuName,"\0");

	char *frequency = malloc(10);
	snprintf(frequency, 10, "%d", CURRENT_SECTION.currentGameNode->data->preferences.frequency);

	int width=calculateProportionalSizeOrDistance(315);
	int height = calculateProportionalSizeOrDistance(72);

	//Main rectangle
	drawRectangleToScreen(width+calculateProportionalSizeOrDistance(4), height+calculateProportionalSizeOrDistance(4), SCREEN_WIDTH/2-(width/2+calculateProportionalSizeOrDistance(2)), SCREEN_HEIGHT/2-(height/2+calculateProportionalSizeOrDistance(2)), (int[]) {37,50,56});

	//Overlay
	drawRectangleToScreen(width, height, SCREEN_WIDTH/2-width/2, SCREEN_HEIGHT/2-height/2, (int[]) {250,250,250});

	//Title Bar
	drawRectangleToScreen(width, height/4, SCREEN_WIDTH/2-width/2, SCREEN_HEIGHT/2-height/2, (int[]){37,50,56});

	//Selection
	if (chosenChoosingOption==0) {
		drawRectangleToScreen(width, height/4, SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2-height/4, (int[]){219,219,219});
	} else if (chosenChoosingOption==1) {
		drawRectangleToScreen(width, height/4, SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2, (int[]){219,219,219});
	} else {
		drawRectangleToScreen(width, height/4, SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2+height/4, (int[]){219,219,219});
	}

	//Name
	char * name = getNameWithoutPath(CURRENT_SECTION.currentGameNode->data->name);
	drawTextOnScreen(getFont(), NULL, calculateProportionalSizeOrDistance(6), (SCREEN_HEIGHT/2)-calculateProportionalSizeOrDistance(28), name, (int[]) {255,255,255}, VAlignMiddle | HAlignLeft);
	free(name);

	TTF_SizeText(getFont(), (const char *) "Overclock: " , &textWidth, NULL);
	textWidth+=calculateProportionalSizeOrDistance(2);

	//Frequency option text
	drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+calculateProportionalSizeOrDistance(4), (SCREEN_HEIGHT/2)-calculateProportionalSizeOrDistance(9), "Overclock: ", textColor, VAlignMiddle | HAlignLeft);
	//Frequency option value
#if defined TARGET_OD_BETA || defined TARGET_RFW || defined TARGET_BITTBOY
	if (CURRENT_SECTION.currentGameNode->data->preferences.frequency==OC_OC) {
		drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth+1, (SCREEN_HEIGHT/2)-calculateProportionalSizeOrDistance(9), "yes", valueColor, VAlignMiddle | HAlignLeft);
	} else {
		drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth+1, (SCREEN_HEIGHT/2)-calculateProportionalSizeOrDistance(9), "no", valueColor, VAlignMiddle | HAlignLeft);
	}
#else
	drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth, (SCREEN_HEIGHT/2)-calculateProportionalSizeOrDistance(9), "unavailable", valueColor, VAlignMiddle | HAlignLeft);
#endif

	drawRectangleToScreen(width, calculateProportionalSizeOrDistance(1), SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2, (int[]){219,219,219});

	//Launch at boot option text
	drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+calculateProportionalSizeOrDistance(4), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(9), "Autostart: ", textColor, VAlignMiddle | HAlignLeft);
	//Launch at boot option value
	if (launchAtBoot) {
		drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth, (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(9), "yes", valueColor, VAlignMiddle | HAlignLeft);
	} else {
		drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth, (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(9), "no", valueColor, VAlignMiddle | HAlignLeft);
	}

	drawRectangleToScreen(width, calculateProportionalSizeOrDistance(1), SCREEN_WIDTH/2-width/2,SCREEN_HEIGHT/2+height/4, (int[]){219,219,219});

	//Emulator option text
	drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+calculateProportionalSizeOrDistance(4), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(27), "Emulator: ", textColor, VAlignMiddle | HAlignLeft);
	//Emulator option value
	drawTextOnScreen(getFont(), NULL, (SCREEN_WIDTH/2)-width/2+textWidth, (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(27), emuName, valueColor, VAlignMiddle | HAlignLeft);

	free(emuName);
	free(frequency);

	logMessage("INFO","Preferences shown");
}

void showConsole() {
	int backgroundColor[3];
	backgroundColor[0]=30;
	backgroundColor[1]=30;
	backgroundColor[2]=130;
	drawRectangleToScreen(calculateProportionalSizeOrDistance(SCREEN_WIDTH), calculateProportionalSizeOrDistance(SCREEN_HEIGHT), 0, 0, backgroundColor);
	if (CURRENT_SECTION.systemLogoSurface!=NULL) {
		displayCenteredSurface(CURRENT_SECTION.systemLogoSurface);
		char *gameCount=malloc(100);
//		snprintf(gameCount,100,"%d GAMES AVAILABLE",theSectionHasGames(&CURRENT_SECTION));
		snprintf(gameCount,100,"%d GAMES AVAILABLE",CURRENT_SECTION.gameCount);


		TTF_Font *font = TTF_OpenFont("/media/data/local/home/.simplemenu/themes/640x480/Epic Cody/resources/general/BebasNeue.otf", calculateProportionalSizeOrDistance(14));
		drawTextOnScreen(font,NULL,calculateProportionalSizeOrDistance(1),calculateProportionalSizeOrDistance(149),gameCount,(int[]){255,174,0},0);
		TTF_CloseFont(font);
		font=NULL;
		free(gameCount);

	} else {
		drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0,0,(int[]){180,180,180});
		drawTextOnScreen(getFont(),NULL,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,CURRENT_SECTION.sectionName,(int[]){0,0,0},VAlignMiddle|HAlignCenter);
	}
	logMessage("INFO","Current console shown");
}

void displayGamePicture(struct Rom *rom) {
	char *pictureWithFullPath=malloc(600);
	char *tempGameName=malloc(300);
	if (favoritesSectionSelected) {
		if (favoritesSize == 0) {
			return;
		}
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		strcpy(pictureWithFullPath, favorite.filesDirectory);
		tempGameName=getGameName(favorite.name);
	} else {
		if (rom==NULL) {
			strcpy(pictureWithFullPath, "NO GAMES FOUND");
			tempGameName=getGameName("NO GAMES FOUND");
		} else {
			strcpy(pictureWithFullPath, rom->directory);
			tempGameName=getGameName(rom->name);
		}
	}
	strcat(pictureWithFullPath,mediaFolder);
	strcat(pictureWithFullPath,"/");
	strcat(pictureWithFullPath,tempGameName);
	strcat(pictureWithFullPath,".png");
	displayBackgroundPicture();
	if (rom==NULL) {
		displayCenteredImageOnScreen(pictureWithFullPath, tempGameName, 1,1);
		return;
	}
	stripGameNameLeaveExtension(tempGameName);
	if (strlen(CURRENT_SECTION.aliasFileName)>1||currentSectionNumber==favoritesSectionNumber) {
		char* displayName=NULL;
		if (rom!=NULL) {
			displayName=getFileNameOrAlias(rom);
		}
		if (!favoritesSectionSelected&&rom!=NULL&&(stripGames||strlen(CURRENT_SECTION.aliasFileName)>1)) {
			if (stripGames) {
				strcpy(displayName,getAliasWithoutAlternateNameOrParenthesis(rom->alias));
			} else {
				strcpy(displayName,rom->alias);
			}
			displayCenteredImageOnScreen(pictureWithFullPath, displayName, 1,1);
			drawPictureTextOnScreen(displayName);
		} else {
			if (favoritesSectionSelected) {
				if (rom!=NULL) {
					if (strlen(rom->alias)<2) {
						char tmp[300];
						strcpy(tmp,getNameWithoutPath(rom->name));
						strcpy(tmp,getNameWithoutExtension(tmp));
						if (stripGames) {
							char * temp1 = getAliasWithoutAlternateNameOrParenthesis(tmp);
							displayCenteredImageOnScreen(pictureWithFullPath, temp1, 1,1);
							drawPictureTextOnScreen(temp1);
							free(temp1);
						} else {
							displayCenteredImageOnScreen(pictureWithFullPath, tmp, 1,1);
							drawPictureTextOnScreen(tmp);
						}
					} else {
						if (stripGames) {
							char * temp1 = getAliasWithoutAlternateNameOrParenthesis(rom->alias);
							displayCenteredImageOnScreen(pictureWithFullPath, temp1, 1,1);
							drawPictureTextOnScreen(temp1);
							free(temp1);
						} else {
							displayCenteredImageOnScreen(pictureWithFullPath, rom->alias, 1,1);
							drawPictureTextOnScreen(rom->alias);
						}
					}
				}
			}
		}
		free(displayName);
	} else {
		if (stripGames) {
			if (rom!=NULL) {
				if (rom->alias==NULL||strlen(rom->alias)<2) {
					displayCenteredImageOnScreen(pictureWithFullPath, tempGameName, 1,1);
					drawPictureTextOnScreen(tempGameName);
				} else {
					displayCenteredImageOnScreen(pictureWithFullPath, rom->alias, 1,1);
					drawPictureTextOnScreen(rom->alias);
				}
			}
		} else {
			if (rom!=NULL) {
				if (rom->alias==NULL||strlen(rom->alias)<2) {
					char tmp[300];
					strcpy(tmp,getNameWithoutPath(rom->name));
					strcpy(tmp,getNameWithoutExtension(tmp));
					displayCenteredImageOnScreen(pictureWithFullPath, tmp, 1,1);
					drawPictureTextOnScreen(tmp);
				} else {
					displayCenteredImageOnScreen(pictureWithFullPath, rom->alias, 1,1);
					drawPictureTextOnScreen(rom->alias);
				}
			}
		}
	}
	if (!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
		if(!favoritesSectionSelected) {
			if (colorfulFullscreenMenu) {
				drawTransparentRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, CURRENT_SECTION.fullScreenMenuBackgroundColor, 180);
				drawTransparentRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, (int[]){0,0,0},100);
			} else {
				drawTransparentRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, (int[]){0,0,0}, 180);
			}
		} else {
			drawTransparentRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, (int[]){0,0,0}, 180);
		}
	}

	free(pictureWithFullPath);
	free(tempGameName);
	logMessage("INFO","Displayed game picture - fullscreen");
}

void displayGamePictureInMenu(struct Rom *rom) {
	char *pictureWithFullPath=malloc(600);
	char *tempGameName=malloc(300);
	if (favoritesSectionSelected) {
		if (favoritesSize == 0) {
			return;
		}
		struct Favorite favorite = favorites[CURRENT_GAME_NUMBER];
		strcpy(pictureWithFullPath, favorite.filesDirectory);
		tempGameName=getGameName(favorite.name);
	} else {
		if (rom==NULL) {
			strcpy(pictureWithFullPath, "NO GAMES FOUND");
			tempGameName=getGameName("NO GAMES FOUND");
		} else {
			strcpy(pictureWithFullPath, rom->directory);
			tempGameName=getGameName(rom->name);
		}
	}
	strcat(pictureWithFullPath,mediaFolder);
	strcat(pictureWithFullPath,"/");
	strcat(pictureWithFullPath,tempGameName);
	strcat(pictureWithFullPath,".png");
	displayImageOnScreenCustom(pictureWithFullPath);
	free(pictureWithFullPath);
	free(tempGameName);
	logMessage("INFO","Displayed game picture");
}

void drawHeader() {
	char finalString [200];
//	if (currentCPU==OC_UC) {
//		strcpy(finalString,"- ");
//		if(currentSectionNumber==favoritesSectionNumber) {
//			strcat(finalString,favorites[CURRENT_GAME_NUMBER].section);
//		} else{
//			if (menuSections[currentSectionNumber].fantasyName!=NULL) {
//				strcat(finalString,menuSections[currentSectionNumber].fantasyName);
//			} else {
//				strcat(finalString,menuSections[currentSectionNumber].sectionName);
//			}
//		}
//		strcat(finalString," -");
//	} else if (currentCPU==OC_NO) {
//		if(currentSectionNumber==favoritesSectionNumber) {
//			strcpy(finalString,favorites[CURRENT_GAME_NUMBER].section);
//		} else{
//			if (strlen(menuSections[currentSectionNumber].fantasyName)>3) {
//				strcat(finalString,menuSections[currentSectionNumber].fantasyName);
//			} else {
//				strcat(finalString,menuSections[currentSectionNumber].sectionName);
//			}
//		}
//	} else {
//		strcpy(finalString,"+ ");
//		if(currentSectionNumber==favoritesSectionNumber) {
//			strcat(finalString,favorites[CURRENT_GAME_NUMBER].section);
//		} else{
//			if (menuSections[currentSectionNumber].fantasyName!=NULL) {
//				strcat(finalString,menuSections[currentSectionNumber].fantasyName);
//			} else {
//				strcat(finalString,menuSections[currentSectionNumber].sectionName);
//			}
//		}
//		strcat(finalString," +");
//	}
	drawTextOnHeader();
}

void drawTimedShutDownScreen() {
	int black[] = {0,0,0};
	drawRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, black);
	char text[30];
	char text1[30];
	char text2[30];
	if (shutDownEnabled) {
		snprintf(text,30,"SHUTTING DOWN");
	} else {
		snprintf(text,30,"QUITTING");
	}
	snprintf(text1,30,"IN %d SECONDS",countDown);
	snprintf(text2,30,"X TO CANCEL");
	genericDrawTextOnScreen(getBigFont(), NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/4, text, (int[]){255,255,255}, VAlignMiddle | HAlignCenter, (int[]){0,0,0}, 0);

	genericDrawTextOnScreen(getBigFont(), NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text1, (int[]){255,255,255}, VAlignMiddle | HAlignCenter, (int[]){0,0,0}, 0);

	genericDrawTextOnScreen(getBigFont(), NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT-SCREEN_HEIGHT/4, text2, (int[]){255,255,255}, VAlignMiddle | HAlignCenter, (int[]){0,0,0}, 0);
}

void drawShutDownScreen() {
	int black[] = {0,0,0};
	drawRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, black);
	if (selectedShutDownOption==1) {
		drawBigWhiteText("REBOOTING");
	} else {
		drawBigWhiteText("SHUTTING DOWN");
	}
}

void drawGameList() {
	currentGameNameBeingDisplayed[0]=' ';
	currentGameNameBeingDisplayed[1]='\0';
	gamesInPage=0;
	int nextLine = 0;
	if(!fullscreenMode) {
		nextLine = calculateProportionalSizeOrDistance(gameListY);
	} else {
		nextLine = calculateProportionalSizeOrDistance(gameListPositionFullScreen);
	}

	char *nameWithoutExtension;
	struct Node* currentNode;
	char *buf;
	currentNode = GetNthNode(ITEMS_PER_PAGE*CURRENT_SECTION.currentPage);
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (currentNode==NULL) {
			break;
		}
		struct Rom* rom = currentNode->data;
		gamesInPage++;
		if (rom->alias!=NULL &&  (strlen(rom->alias)>2)) {
			nameWithoutExtension=malloc(strlen(rom->alias)+1);
			strcpy(nameWithoutExtension,rom->alias);
			if(stripGames) {
				char* temp1 = getAliasWithoutAlternateNameOrParenthesis(rom->alias);
				free(nameWithoutExtension);
				nameWithoutExtension=malloc(strlen(temp1)+1);
				strcpy(nameWithoutExtension,temp1);
				free(temp1);
			}
		} else {
			nameWithoutExtension=malloc(strlen(rom->name)+1);
			strcpy(nameWithoutExtension,rom->name);
			if(stripGames) {
				char* temp1 = strdup(nameWithoutExtension);
				stripGameName(temp1);
				free(nameWithoutExtension);
				nameWithoutExtension=malloc(strlen(temp1)+1);
				strcpy(nameWithoutExtension,temp1);
				free(temp1);
			} else {
				char *nameWithoutPath=getNameWithoutPath(nameWithoutExtension);
				free(nameWithoutExtension);
				nameWithoutExtension=getNameWithoutExtension(nameWithoutPath);
				free(nameWithoutPath);
			}
		}
		buf=strdup(nameWithoutExtension);

		char *temp = malloc(strlen(buf)+2);
		if (rom->preferences.frequency == OC_UC) {
			strcpy(temp,"-");
			strcat(temp,buf);
		} else 	if (rom->preferences.frequency == OC_OC) {
			strcpy(temp,"+");
			strcat(temp,buf);
		} else {
			strcpy(temp,buf);
		}

		if (i==menuSections[currentSectionNumber].currentGameInPage) {
			if(strlen(buf)>0) {
				if(fullscreenMode) {
					if(!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
						drawShadedGameNameOnScreenPicMode(temp, nextLine);
					}
				} else {
					MAGIC_NUMBER = calculateProportionalSizeOrDistance(gameListWidth);
					strcpy(currentGameNameBeingDisplayed,temp);
					drawShadedGameNameOnScreenCustom(temp, nextLine);
				}
			}
		} else {
			if(strlen(buf)>0) {
				if(fullscreenMode) {
					if(!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
						drawNonShadedGameNameOnScreenPicMode(temp, nextLine);
					}
				} else {
					MAGIC_NUMBER = calculateProportionalSizeOrDistance(gameListWidth);
					drawNonShadedGameNameOnScreenCustom(temp, nextLine);
				}
			}
		}
		if (!fullscreenMode) {
			nextLine+=calculateProportionalSizeOrDistance(itemsSeparation);
		} else {
			nextLine+=calculateProportionalSizeOrDistance(itemsSeparation);
		}
		free(nameWithoutExtension);
		free(buf);
		free(temp);
		currentNode = currentNode->next;
	}
	MAGIC_NUMBER = SCREEN_WIDTH-calculateProportionalSizeOrDistance(2);
	logMessage("INFO","Game list - Done");
}

void setupDecorations() {
	if (text1X!=-1&&text1Y!=-1) {
		drawHeader();
	}
	char *gameNumber=malloc(10);
	snprintf(gameNumber,10,"%d/%d",CURRENT_SECTION.gameCount>0?(CURRENT_SECTION.currentGameInPage+ITEMS_PER_PAGE*CURRENT_SECTION.currentPage)+1:0,CURRENT_SECTION.gameCount);
	drawCustomGameNumber(gameNumber, calculateProportionalSizeOrDistance(text2X), calculateProportionalSizeOrDistance(text2Y));
	free(gameNumber);
	logMessage("INFO","Decorations set");
}

void setOptionsAndValues (char **options, char **values, char **hints){

	options[TIDY_ROMS_OPTION]= malloc(100);
	options[FULL_SCREEN_FOOTER_OPTION]= malloc(100);
	options[FULL_SCREEN_MENU_OPTION]= malloc(100);
	options[THEME_OPTION]= malloc(100);
	options[SCREEN_TIMEOUT_OPTION]= malloc(100);
	options[DEFAULT_OPTION]= malloc(100);
	options[USB_OPTION]= malloc(100);
	options[SHUTDOWN_OPTION]= malloc(100);
	options[AUTO_HIDE_LOGOS_OPTION]= malloc(100);

	values[TIDY_ROMS_OPTION]= malloc(10);
	values[FULL_SCREEN_FOOTER_OPTION]= malloc(10);
	values[FULL_SCREEN_MENU_OPTION]= malloc(10);
	values[THEME_OPTION]= malloc(2000);
	values[SCREEN_TIMEOUT_OPTION]= malloc(40);
	values[DEFAULT_OPTION]= malloc(4);
	values[USB_OPTION]= malloc(100);
	values[SHUTDOWN_OPTION]= malloc(30);
	values[AUTO_HIDE_LOGOS_OPTION]= malloc(10);

	hints[TIDY_ROMS_OPTION]= malloc(100);
	hints[FULL_SCREEN_FOOTER_OPTION]= malloc(100);
	hints[FULL_SCREEN_MENU_OPTION]= malloc(100);
	hints[THEME_OPTION]= malloc(100);
	hints[SCREEN_TIMEOUT_OPTION]= malloc(100);
	hints[DEFAULT_OPTION]= malloc(100);
	hints[USB_OPTION]= malloc(100);
	hints[SHUTDOWN_OPTION]= malloc(100);
	hints[AUTO_HIDE_LOGOS_OPTION]= malloc(100);

	strcpy(options[TIDY_ROMS_OPTION],"Tidy rom names ");
	strcpy(options[FULL_SCREEN_FOOTER_OPTION],"Fullscreen rom names ");
	strcpy(options[FULL_SCREEN_MENU_OPTION],"Fullscreen menu ");
	logMessage("INFO","Full screen menu option");
	logMessage("INFO",options[FULL_SCREEN_MENU_OPTION]);
	strcpy(options[THEME_OPTION],"Theme ");
	strcpy(options[SCREEN_TIMEOUT_OPTION],"Screen timeout ");
	strcpy(options[DEFAULT_OPTION],"Default launcher  ");
	logMessage("INFO","Default option");
	logMessage("INFO",options[DEFAULT_OPTION]);
	#if defined TARGET_RFW
	strcpy(options[USB_OPTION],"USB mode");
	#else
	strcpy(options[USB_OPTION],"HDMI ");
	#endif
	strcpy(options[AUTO_HIDE_LOGOS_OPTION],"Auto-hide logos ");

	strcpy(options[SHUTDOWN_OPTION],"Session ");
	strcpy(hints[TIDY_ROMS_OPTION],"CUT DETAILS OUT OF ROM NAMES");
	strcpy(hints[FULL_SCREEN_FOOTER_OPTION],"DISPLAY THE CURRENT ROM NAME");
	strcpy(hints[FULL_SCREEN_MENU_OPTION],"DISPLAY A TRANSLUCENT MENU");
	logMessage("INFO","Full screen menu hint");
	logMessage("INFO",hints[FULL_SCREEN_MENU_OPTION]);
	strcpy(hints[THEME_OPTION],"LAUNCHER THEME");
	strcpy(hints[SCREEN_TIMEOUT_OPTION],"SECONDS UNTIL THE SCREEN TURNS OFF");
	strcpy(hints[DEFAULT_OPTION],"LAUNCH AFTER BOOTING");
	logMessage("INFO","Default option hint");
	logMessage("INFO",hints[DEFAULT_OPTION]);
	strcpy(hints[AUTO_HIDE_LOGOS_OPTION],"HIDE LOGOS AFTER A SECOND");

	#if defined TARGET_RFW
	strcpy(hints[USB_OPTION],"PRESS A TO ENABLE USB");
	#else
	strcpy(hints[USB_OPTION],"ENABLE OR DISABLE HDMI");
	#endif
	strcpy(hints[SHUTDOWN_OPTION],"A TO CONFIRM - LEFT/RIGHT TO CHOOSE");
	if (shutDownEnabled) {
		switch (selectedShutDownOption) {
			case 0:
				strcpy(values[SHUTDOWN_OPTION],"shutdown");
				break;
			case 1:
				strcpy(values[SHUTDOWN_OPTION],"reboot");
				break;
		}
	} else {
		switch (selectedShutDownOption) {
			case 0:
				strcpy(values[SHUTDOWN_OPTION],"quit");
				break;
			case 1:
				strcpy(values[SHUTDOWN_OPTION],"reboot");
				break;
			case 2:
				strcpy(values[SHUTDOWN_OPTION],"shutdown");
				break;
		}
	}


	if (stripGames) {
		strcpy(values[TIDY_ROMS_OPTION],"enabled");
	} else {
		strcpy(values[TIDY_ROMS_OPTION],"disabled");
	}
	if (footerVisibleInFullscreenMode) {
		strcpy(values[FULL_SCREEN_FOOTER_OPTION],"enabled");
	} else {
		strcpy(values[FULL_SCREEN_FOOTER_OPTION],"disabled");
	}
	if (menuVisibleInFullscreenMode) {
		strcpy(values[FULL_SCREEN_MENU_OPTION],"enabled");
	} else {
		strcpy(values[FULL_SCREEN_MENU_OPTION],"disabled");
	}
	logMessage("INFO","Full screen menu is...");
	logMessage("INFO",values[FULL_SCREEN_MENU_OPTION]);
	char *themeName=getNameWithoutPath((themes[activeTheme]));
	strcpy(values[THEME_OPTION],themeName);
	free(themeName);
	if (timeoutValue>0&&hdmiEnabled==0) {
		sprintf(values[SCREEN_TIMEOUT_OPTION],"%d",timeoutValue);
	} else {
		sprintf(values[SCREEN_TIMEOUT_OPTION],"%s","always on");
	}

	if (shutDownEnabled) {
		strcpy(values[DEFAULT_OPTION],"yes");
		logMessage("INFO","Default option value");
		logMessage("INFO",values[DEFAULT_OPTION]);
	} else {
		strcpy(values[DEFAULT_OPTION],"no");
		logMessage("INFO","Default option value");
		logMessage("INFO",values[DEFAULT_OPTION]);
	}
	#if defined TARGET_RFW
	strcpy(values[USB_OPTION],"YES \0");
	#else
	if (hdmiChanged==1) {
		strcpy(values[USB_OPTION],"enabled");
	} else {
		strcpy(values[USB_OPTION],"disabled");
	}
	#endif
	if (autoHideLogos) {
		strcpy(values[AUTO_HIDE_LOGOS_OPTION],"enabled");
	} else {
		strcpy(values[AUTO_HIDE_LOGOS_OPTION],"disabled");
	}
}

void drawSettingsScreen() {
	SHUTDOWN_OPTION=0;
	THEME_OPTION=1;
	SCREEN_TIMEOUT_OPTION=2;
	TIDY_ROMS_OPTION=3;
	AUTO_HIDE_LOGOS_OPTION=4;
	FULL_SCREEN_FOOTER_OPTION=5;
	FULL_SCREEN_MENU_OPTION=6;
	DEFAULT_OPTION=7;
	USB_OPTION=8;

	int headerAndFooterBackground[3]={37,50,56};
	int headerAndFooterText[3]={255,255,255};
	int bodyText[3]= {90,90,90};
	int bodyHighlightedText[3]= {0,147,131};
	int bodyBackground[3]={250,250,250};

	int batteryLevel90to100[] = {1,255,1};
	int batteryLevel80to90[] = {51,255,0};
	int batteryLevel70to80[] = {101,255,0};
	int batteryLevel60to70[] = {153,254,0};
	int batteryLevel50to60[] = {202,253,0};
	int batteryLevel40to50[] = {255,254,3};
	int batteryLevel30to40[] = {255,204,0};
	int batteryLevel20to30[] = {255,152,1};
	int batteryLevel10to20[] = {255,102,0};
	int batteryLevel0to10[] = {255,51,0};


	int gray5[]={121, 121, 121};

	int *levels[11];
	levels[0] = batteryLevel0to10;
	levels[1] = batteryLevel10to20;
	levels[2] = batteryLevel20to30;
	levels[3] = batteryLevel30to40;
	levels[4] = batteryLevel40to50;
	levels[5] = batteryLevel50to60;
	levels[6] = batteryLevel60to70;
	levels[7] = batteryLevel70to80;
	levels[8] = batteryLevel80to90;
	levels[9] = batteryLevel90to100;
	levels[10] = batteryLevel90to100;

	char *options[10];
	char *values[10];
	char *hints[10];

	logMessage("INFO","Setting options and values");
	setOptionsAndValues(options, values, hints);

	logMessage("INFO","Drawing shit");
	drawRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), 0,calculateProportionalSizeOrDistance(22), bodyBackground);
	drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(42), 0, 0, headerAndFooterBackground);
	drawTextOnSettingsHeaderLeftWithColor("SETTINGS",headerAndFooterText);

	logMessage("INFO","Positioning batt");
	int pos = (lastChargeLevel/10);

	drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(4), 0, calculateProportionalSizeOrDistance(42), gray5);

	logMessage("INFO","Positioning batt 1");
	for (int i=pos;i>=0;i--) {
		drawRectangleToScreen(SCREEN_WIDTH/10, calculateProportionalSizeOrDistance(4), (SCREEN_WIDTH/10)*i, calculateProportionalSizeOrDistance(42), levels[i]);
	}
	int nextLine = calculateProportionalSizeOrDistance(50);
	int nextLineText = calculateProportionalSizeOrDistance(50);
	int selected=0;
	#if defined TARGET_RFW
	int max = 9;
	#elif defined TARGET_OD || defined TARGET_OD_BETA || defined TARGET_PC
	int max = 9;
	#else
	int max = 8;
	#endif
	logMessage("INFO","Defining number of items");
	logMessage("INFO","About to go through items");
	for (int i=0;i<max;i++) {
		logMessage("INFO","one item");
		char temp[300];
		strcpy(temp,options[i]);
		if(strlen(values[i])>0) {
			strcat(temp,values[i]);
		}
		if(i==chosenSetting) {
			logMessage("INFO","Chosen setting");
			logMessage("INFO",options[i]);
			logMessage("INFO",values[i]);
//			drawRectangleToScreen(5, 20, 0, nextLine-3, bodyHighlightedText);
			int lineColor[] = { 219,219,219};
			if (i==0) {
				drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(19), 0, nextLine-calculateProportionalSizeOrDistance(4), lineColor);
			} else if (i==max){
				drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(21), 0, nextLine-calculateProportionalSizeOrDistance(4), lineColor);
			} else {
				drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(20), 0, nextLine-calculateProportionalSizeOrDistance(4), lineColor);
			}
			drawNonShadedSettingsOptionOnScreen(options[i], nextLineText, bodyText);
			drawShadedSettingsOptionValueOnScreen(options[i],values[i], nextLineText, bodyHighlightedText,lineColor);
			selected=i;
		} else {
			logMessage("INFO","Non-Chosen setting");
			logMessage("INFO",options[i]);
			logMessage("INFO",values[i]);
			drawNonShadedSettingsOptionOnScreen(options[i], nextLineText, bodyText);
			drawSettingsOptionValueOnScreen(options[i],values[i], nextLineText, bodyHighlightedText);
		}
		int lineColor[] = { 229,229,229};
		logMessage("INFO","Drawing rect");
		if (i<max)  {
			drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(1), 0, nextLine+calculateProportionalSizeOrDistance(15), lineColor);
		}
		logMessage("INFO","Next line 1");
		nextLine+=calculateProportionalSizeOrDistance(19);
		logMessage("INFO","Next line 2");
		nextLineText+=calculateProportionalSizeOrDistance(19);
		logMessage("INFO","---");
	}
	logMessage("INFO","Out of the loop");
	drawRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), headerAndFooterBackground);
	logMessage("INFO","Drawing Footer");
	drawTextOnSettingsFooterWithColor(hints[selected], bodyBackground);
	logMessage("INFO","Freeign options");
	free(options[TIDY_ROMS_OPTION]);
	free(options[FULL_SCREEN_FOOTER_OPTION]);
	free(options[FULL_SCREEN_MENU_OPTION]);
	free(options[THEME_OPTION]);
	free(options[SCREEN_TIMEOUT_OPTION]);
	free(options[DEFAULT_OPTION]);
	free(options[SHUTDOWN_OPTION]);
	free(options[USB_OPTION]);
	free(options[AUTO_HIDE_LOGOS_OPTION]);

	logMessage("INFO","Freeign values");
	free(values[TIDY_ROMS_OPTION]);
	free(values[FULL_SCREEN_FOOTER_OPTION]);
	free(values[FULL_SCREEN_MENU_OPTION]);
	free(values[THEME_OPTION]);
	free(values[SCREEN_TIMEOUT_OPTION]);
	free(values[DEFAULT_OPTION]);
	free(values[SHUTDOWN_OPTION]);
	free(values[USB_OPTION]);
	free(values[AUTO_HIDE_LOGOS_OPTION]);

	logMessage("INFO","Freeign hints");
	free(hints[TIDY_ROMS_OPTION]);
	free(hints[FULL_SCREEN_FOOTER_OPTION]);
	free(hints[FULL_SCREEN_MENU_OPTION]);
	free(hints[THEME_OPTION]);
	free(hints[SCREEN_TIMEOUT_OPTION]);
	free(hints[DEFAULT_OPTION]);
	free(hints[SHUTDOWN_OPTION]);
	free(hints[USB_OPTION]);
	free(hints[AUTO_HIDE_LOGOS_OPTION]);
	logMessage("INFO","Settings drawn");
}

void updateScreen(struct Node *node) {
	struct Rom *rom;
	if (node==NULL) {
		rom = NULL;
	} else {
		rom = node->data;
	}
	if (!isUSBMode&&!itsStoppedBecauseOfAnError) {
		switch(currentState) {
			case BROWSING_GAME_LIST:
				if (fullscreenMode) {
					logMessage("INFO","Fullscreen mode");
					displayGamePicture(rom);
					drawGameList();
					displayHeart(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
				} else {
					logMessage("INFO","Menu mode");
					displayCenteredSurface(CURRENT_SECTION.backgroundSurface);
					drawGameList();
					displayGamePictureInMenu(rom);
					setupDecorations(rom);
				}
				if (CURRENT_SECTION.alphabeticalPaging) {
					logMessage("INFO","Show alphabetical navigation bar");
					if (rom->name!=NULL) {
						showLetter(rom);
					}
				}
				if (currentSectionNumber != favoritesSectionNumber && CURRENT_SECTION.gameCount==0) {
					generateError("NO GAMES FOUND-FOR THIS SECTION GROUP", 0);
					showErrorMessage(errorMessage);
				}
				break;
			case SETTINGS_SCREEN:
				drawSettingsScreen();
				break;
			case CHOOSING_GROUP:
				showCurrentGroup();
				break;
			case SELECTING_EMULATOR:
				showRomPreferences();
				break;
			case SELECTING_SECTION:
				showConsole();
				break;
			case SHUTTING_DOWN:
				drawShutDownScreen();
				break;
			case AFTER_RUNNING_LAUNCH_AT_BOOT:
				drawTimedShutDownScreen();
				break;
		}
	} else if (isUSBMode) {
		drawUSBScreen();
	} else if (itsStoppedBecauseOfAnError) {
		showErrorMessage(errorMessage);
	}
}

void setupKeys() {
	initializeKeys();
	logMessage("INFO","Input successfully configured");
}

void clearShutdownTimer() {
	if (shutdownTimer != NULL) {
		SDL_RemoveTimer(shutdownTimer);
	}
	shutdownTimer = NULL;
}

uint32_t countDownToShutdown() {
	countDown--;
	if(countDown==0) {
		running=0;
		clearShutdownTimer();
		pushEvent();
		return 0;
	}
	updateScreen(NULL);
	refreshScreen();
	return 1000;
}

void resetShutdownTimer() {
	countDown=3;
	clearShutdownTimer();
	shutdownTimer=SDL_AddTimer(1 * 1e3, countDownToShutdown, NULL);
	updateScreen(NULL);
	refreshScreen();
}

void clearPicModeHideMenuTimer() {
	if (picModeHideMenuTimer != NULL) {
		SDL_RemoveTimer(picModeHideMenuTimer);
	}
	picModeHideMenuTimer = NULL;
}

uint32_t hideFullScreenModeMenu() {
	if(!hotKeyPressed) {
		clearPicModeHideMenuTimer();
		isPicModeMenuHidden=1;
	}
	pushEvent();
	return 0;
}

void resetPicModeHideMenuTimer() {
	if (menuVisibleInFullscreenMode) {
		isPicModeMenuHidden=0;
		clearPicModeHideMenuTimer();
		picModeHideMenuTimer=SDL_AddTimer(0.6 * 1e3, hideFullScreenModeMenu, NULL);
	}
}

void clearPicModeHideLogoTimer() {
	if (picModeHideLogoTimer != NULL) {
		SDL_RemoveTimer(picModeHideLogoTimer);
	}
	picModeHideLogoTimer = NULL;
}

uint32_t hidePicModeLogo() {
	clearPicModeHideLogoTimer();
	hotKeyPressed=0;
	aKeyComboWasPressed=0;
	if (CURRENT_SECTION.backgroundSurface==NULL) {
		logMessage("INFO","Loading system background");
		CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
		resizeSectionBackground(&CURRENT_SECTION);
		logMessage("INFO","Loading system picture");
		CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
		resizeSectionSystemPicture(&CURRENT_SECTION);
	}
	currentState=BROWSING_GAME_LIST_AFTER_TIMER;
	pushEvent();
	return 0;
}

void resetPicModeHideLogoTimer() {
	clearPicModeHideLogoTimer();
	picModeHideLogoTimer=SDL_AddTimer(0.7 * 1e3, hidePicModeLogo, NULL);
}

void clearHideHeartTimer() {
	if (hideHeartTimer != NULL) {
		SDL_RemoveTimer(hideHeartTimer);
	}
	hideHeartTimer = NULL;
}

uint32_t hideHeart() {
	clearHideHeartTimer();
	if(!isPicModeMenuHidden) {
		resetPicModeHideMenuTimer();
	}
	pushEvent();
	return 0;
}

void resetHideHeartTimer() {
	clearHideHeartTimer();
	hideHeartTimer=SDL_AddTimer(0.5 * 1e3, hideHeart, NULL);
}

uint32_t controlsCallBack() {
	return 16.666667;
}


void startControlsTimer() {
	screenTimer=SDL_AddTimer(16.666667, controlsCallBack, NULL);
}
