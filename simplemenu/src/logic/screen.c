#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../headers/constants.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/string_utils.h"
#include "../headers/opk.h"
#include "../headers/doubly_linked_rom_list.h"


char buf[300];

void displayBackgroundPicture() {
	if(fullscreenMode) {
		displayCenteredImageOnScreen(fullscreenBackground, " ",1);
	} else {
		displayCenteredImageOnScreen(simpleBackground, " ",1);
	}
}


void showErrorMessage(char *errorMessage) {
	int width = (((strlen(errorMessage))*(180))/calculateProportionalSizeOrDistance(18));
	int height = calculateProportionalSizeOrDistance(40);
	if(strchr(errorMessage,'-')!=NULL) {
		height = calculateProportionalSizeOrDistance(60);
		width = (((strlen(errorMessage)/2)*calculateProportionalSizeOrDistance(200))/calculateProportionalSizeOrDistance(18))+calculateProportionalSizeOrDistance(20);
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
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width+10), calculateProportionalSizeOrDistance(height+10), SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(width/2)-calculateProportionalSizeOrDistance(5),SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(height/2)-calculateProportionalSizeOrDistance(5), borderColor);
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width), calculateProportionalSizeOrDistance(height), SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(width/2),SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(height/2), filling);
	drawError(errorMessage, textColor);
	itsStoppedBecauseOfAnError=1;
}

int letterExistsInGameList(char *letter, char* letters) {
	if (strstr(letters,letter)!=NULL) {
		return 1;
	}
	return 0;
}

void showLetter(struct Rom *rom) {
	int rectangleWidth = 80;
	int rectangleHeight = 80;
	int rectangleX = (SCREEN_WIDTH/2);
	int rectangleY = (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3);
	int filling[3];
	int borderColor[3];
	borderColor[0]=CURRENT_SECTION.headerAndFooterBackgroundColor[0]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[0]+45;
	borderColor[1]=CURRENT_SECTION.headerAndFooterBackgroundColor[1]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[1]+45;
	borderColor[2]=CURRENT_SECTION.headerAndFooterBackgroundColor[2]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[2]+45;
	filling[0]=CURRENT_SECTION.headerAndFooterBackgroundColor[0];
	filling[1]=CURRENT_SECTION.headerAndFooterBackgroundColor[1];
	filling[2]=CURRENT_SECTION.headerAndFooterBackgroundColor[2];
	int textColor[3] = {CURRENT_SECTION.headerAndFooterTextColor[0], CURRENT_SECTION.headerAndFooterTextColor[1], CURRENT_SECTION.headerAndFooterTextColor[2]};
	if (fullscreenMode) {
		filling[0] = 0;
		filling[1] = 0;
		filling[2] = 0;
		borderColor[0]=255;
		borderColor[1]=255;
		borderColor[2]=255;
		textColor[0]=255;
		textColor[1]=255;
		textColor[2]=255;
		rectangleWidth = calculateProportionalSizeOrDistance(320);
		rectangleHeight=calculateProportionalSizeOrDistance(20);
		rectangleX = 0;
		rectangleY = calculateProportionalSizeOrDistance(220);
	}
	if (ITEMS_PER_PAGE==12||ITEMS_PER_PAGE==16) {
		filling[0] = CURRENT_SECTION.headerAndFooterBackgroundColor[0];
		filling[1] = CURRENT_SECTION.headerAndFooterBackgroundColor[1];
		filling[2] = CURRENT_SECTION.headerAndFooterBackgroundColor[2];
		borderColor[0]=255;
		borderColor[1]=255;
		borderColor[2]=255;
		textColor[0]=CURRENT_SECTION.headerAndFooterTextColor[0];
		textColor[1]=CURRENT_SECTION.headerAndFooterTextColor[0];
		textColor[2]=CURRENT_SECTION.headerAndFooterTextColor[0];
		rectangleWidth = calculateProportionalSizeOrDistance(320);
		rectangleHeight=calculateProportionalSizeOrDistance(21);
		rectangleX = 0;
		rectangleY = calculateProportionalSizeOrDistance(219);
	}
	if (!fullscreenMode&&ITEMS_PER_PAGE==10) {
		drawRectangleOnScreen(calculateProportionalSizeOrDistance(rectangleWidth+10), calculateProportionalSizeOrDistance(rectangleHeight+10), SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(rectangleWidth/2)-calculateProportionalSizeOrDistance(5),SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(rectangleHeight/2)-calculateProportionalSizeOrDistance(5), borderColor);
		drawRectangleOnScreen(calculateProportionalSizeOrDistance(rectangleWidth), calculateProportionalSizeOrDistance(rectangleHeight), SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(rectangleWidth/2),SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(rectangleHeight/2), filling);
	} else {
		drawRectangleOnScreen(rectangleWidth, rectangleHeight, rectangleX, rectangleY, filling);
		drawTransparentRectangleToScreen(rectangleWidth, rectangleHeight, rectangleX, rectangleY, filling,80);
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
	if (fullscreenMode||MENU_ITEMS_PER_PAGE==12||MENU_ITEMS_PER_PAGE==16) {
		int x = 0;
		int y = calculateProportionalSizeOrDistance(231);
		if (MENU_ITEMS_PER_PAGE==12&&!fullscreenMode) {
			y = calculateProportionalSizeOrDistance(231);
		}
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
			drawCurrentLetter(letters[i], textColor, x, y);
		}
	} else {
		drawCurrentLetter(currentGameFirstLetter, textColor, rectangleX, rectangleY);
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
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(SCREEN_WIDTH), calculateProportionalSizeOrDistance(height), 0, 0, backgroundColor);
	displayCenteredImageOnScreen(sectionGroups[activeGroup].groupBackground," ",1);
	drawTransparentRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(70), 0, SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(38), backgroundColor, 50);
	drawCurrentSectionGroup(tempString, textColor);
	free(tempString);
}

void showCurrentEmulator() {
	int height = 30;
	int filling[3];
	int borderColor[3];
	borderColor[0]=CURRENT_SECTION.headerAndFooterBackgroundColor[0]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[0]+45;
	borderColor[1]=CURRENT_SECTION.headerAndFooterBackgroundColor[1]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[1]+45;
	borderColor[2]=CURRENT_SECTION.headerAndFooterBackgroundColor[2]+45>255?255:CURRENT_SECTION.headerAndFooterBackgroundColor[2]+45;
	filling[0]=CURRENT_SECTION.headerAndFooterBackgroundColor[0];
	filling[1]=CURRENT_SECTION.headerAndFooterBackgroundColor[1];
	filling[2]=CURRENT_SECTION.headerAndFooterBackgroundColor[2];
	int textColor[3]= {CURRENT_SECTION.headerAndFooterTextColor[0], CURRENT_SECTION.headerAndFooterTextColor[1], CURRENT_SECTION.headerAndFooterTextColor[2]};
	if (fullscreenMode) {
		filling[0] = 21;
		filling[1] = 18;
		filling[2] = 26;
		borderColor[0]=255;
		borderColor[1]=255;
		borderColor[2]=255;
		textColor[0]=255;
		textColor[1]=255;
		textColor[2]=255;
	}
	//	char *tempString = malloc(strlen(CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory])+strlen(CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable])+1);
	char *tempString = malloc(strlen(CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable])+1);
	//	strcpy(tempString,CURRENT_SECTION.emulatorDirectories[CURRENT_SECTION.activeEmulatorDirectory]);
	strcpy(tempString,CURRENT_SECTION.executables[CURRENT_SECTION.activeExecutable]);
	strcat(tempString,"\0");
	int width = strlen(tempString)*11;
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width+10), calculateProportionalSizeOrDistance(height+10), SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(width/2)-calculateProportionalSizeOrDistance(5),SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(height/2)-calculateProportionalSizeOrDistance(5)  , borderColor);
	drawRectangleOnScreen(calculateProportionalSizeOrDistance(width)   , calculateProportionalSizeOrDistance(height)   , SCREEN_WIDTH/2-calculateProportionalSizeOrDistance(width/2)                                       ,SCREEN_HEIGHT/2-calculateProportionalSizeOrDistance(height/2)                                         , filling);
	drawCurrentExecutable(tempString, textColor);
	free(tempString);
}

void showConsole() {
	displayCenteredImageOnScreen(CURRENT_SECTION.systemLogo, CURRENT_SECTION.sectionName, 1);
}

//void showHeart() {
//	displayCenteredImageOnScreen(favoriteIndicator, " ", 0);
//}

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
	if (rom!=NULL) {
		char *tempDisplayName = getFileNameOrAlias(rom);
		displayCenteredImageOnScreen(pictureWithFullPath, tempDisplayName, 1);
		free(tempDisplayName);
	} else {
		displayCenteredImageOnScreen(pictureWithFullPath, tempGameName, 1);
	}
	if (!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
		int black[3]={0, 0, 0};
		drawTransparentRectangleToScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, black, 210);
	} else {
		int black[3]={0, 0, 0};
		if(footerVisibleInFullscreenMode) {
			drawTransparentRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(19), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(19), black, 180);
		}
		stripGameNameLeaveExtension(tempGameName);
		if (strlen(CURRENT_SECTION.aliasFileName)>1||currentSectionNumber==favoritesSectionNumber) {
			char* displayName=NULL;
			if (rom!=NULL) {
				displayName=getFileNameOrAlias(rom);
			}
			if (rom!=NULL&&(stripGames||strlen(CURRENT_SECTION.aliasFileName)>1)) {
				if (stripGames) {
					strcpy(displayName,getAliasWithoutAlternateNameOrParenthesis(rom->alias));
				}
				drawPictureTextOnScreen(displayName);
			} else {
				if (currentSectionNumber==favoritesSectionNumber) {
					if (rom!=NULL) {
						if (strlen(rom->alias)<2) {
							char tmp[300];
							strcpy(tmp,getNameWithoutPath(rom->name));
							strcpy(tmp,getNameWithoutExtension(tmp));
							drawPictureTextOnScreen(tmp);
						} else {
							drawPictureTextOnScreen(rom->alias);
						}
					}
				}
			}
			free(displayName);
		} else {
			if (stripGames) {
				if (rom!=NULL) {
					if (rom->alias==NULL||strlen(rom->alias)<2) {
						drawPictureTextOnScreen(tempGameName);
					} else {
						drawPictureTextOnScreen(rom->alias);
					}
				}
			} else {
				if (rom!=NULL) {
					if (rom->alias==NULL||strlen(rom->alias)<2) {
						char tmp[300];
						strcpy(tmp,getNameWithoutPath(rom->name));
						strcpy(tmp,getNameWithoutExtension(tmp));
						drawPictureTextOnScreen(tmp);
					} else {
						drawPictureTextOnScreen(rom->alias);
					}
				}
			}
		}
	}

	free(pictureWithFullPath);
	free(tempGameName);
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
	if (rom!=NULL) {
		char *tempDisplayName = getFileNameOrAlias(rom);
		if (MENU_ITEMS_PER_PAGE==12) {
			displayImageOnScreenTraditional(pictureWithFullPath);
		} else {
			displayImageOnScreenDrunkenMonkey(pictureWithFullPath);
		}
		free(tempDisplayName);
	} else {
		if (MENU_ITEMS_PER_PAGE==12) {
			displayImageOnScreenTraditional(pictureWithFullPath);
		} else {
			displayImageOnScreenDrunkenMonkey(pictureWithFullPath);
		}
	}
	free(pictureWithFullPath);
	free(tempGameName);
}

void drawHeader(struct Rom *rom) {
	char finalString [100];
	//	char timeString[150];
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterBackgroundColor[0],menuSections[currentSectionNumber].headerAndFooterBackgroundColor[1],menuSections[currentSectionNumber].headerAndFooterBackgroundColor[2]};
	if (!fullscreenMode) {
		if(MENU_ITEMS_PER_PAGE!=12) {
			drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance((22*fontSize)/14), 0, 0, rgbColor);
		} else {
			drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, rgbColor);
		}
	}
	if (currentCPU==OC_UC) {
		strcpy(finalString,"- ");
		strcat(finalString,menuSections[currentSectionNumber].sectionName);
		strcat(finalString," -");
	} else if (currentCPU==OC_NO) {
		if(currentSectionNumber==favoritesSectionNumber) {
			if(rom!=NULL) {
				strcpy(finalString,favorites[CURRENT_GAME_NUMBER].section);
			} else {
				strcpy(finalString,CURRENT_SECTION.sectionName);
			}
		} else{
			strcpy(finalString,menuSections[currentSectionNumber].sectionName);
		}
	} else {
		strcpy(finalString,"+ ");
		strcat(finalString,menuSections[currentSectionNumber].sectionName);
		strcat(finalString," +");
	}
	//	strcpy(timeString,(asctime(currTime))+11);
	//	timeString[strlen(timeString)-9]='\0';
	//	drawTimeOnFooter(timeString);
	//	char str[20];
	//	if (lastChargeLevel==-1) {
	//		sprintf(str, "%s", "CHARGING");
	//	} else {
	//		sprintf(str, "%d%%", lastChargeLevel);
	//	}
	//	drawBatteryOnFooter(str);
	drawTextOnHeader(finalString);
}

void drawShutDownScreen() {
	int black[] = {0,0,0};
	drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, black);
	drawShutDownText("SHUTTING DOWN");
}

void drawGameList() {
	int rgbColor[] = {menuSections[currentSectionNumber].bodyBackgroundColor[0],menuSections[currentSectionNumber].bodyBackgroundColor[1],menuSections[currentSectionNumber].bodyBackgroundColor[2]};
	if (!fullscreenMode) {
		if(MENU_ITEMS_PER_PAGE!=12) {
			drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance((43*fontSize)/14), 0, calculateProportionalSizeOrDistance((22*fontSize)/14), rgbColor);
		} else {
			drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43), 0, calculateProportionalSizeOrDistance(22), rgbColor);
		}
	}
	gamesInPage=0;
	int nextLine = calculateProportionalSizeOrDistance((10*((14*29)/fontSize))/ITEMS_PER_PAGE);//CHANGE FIRST VALUE FOR FONT SIZE
	if(MENU_ITEMS_PER_PAGE==16) {
		nextLine -= calculateProportionalSizeOrDistance(7);
	} else if (MENU_ITEMS_PER_PAGE==10) {
		nextLine -= calculateProportionalSizeOrDistance(1);
	}
	if (fullscreenMode) {
		nextLine = calculateProportionalSizeOrDistance(fontSize-2);//CHANGE FIRST VALUE FOR FONT SIZE
	}
	char *nameWithoutExtension;
	struct Node* currentNode;
	currentNode = GetNthNode(ITEMS_PER_PAGE*CURRENT_SECTION.currentPage);
	for (int i=0;i<ITEMS_PER_PAGE;i++) {
		if (currentNode==NULL) {
			break;
		}
		struct Rom* rom = currentNode->data;
		gamesInPage++;
		sprintf(buf,"%s", "");
		if (rom->alias!=NULL &&  (strlen(rom->alias)>2)) {
			nameWithoutExtension=malloc(strlen(rom->alias)+1);
			strcpy(nameWithoutExtension,rom->alias);
			if(stripGames) {
				strcpy(nameWithoutExtension,getAliasWithoutAlternateNameOrParenthesis(rom->alias));
//				stripGameNameLeaveExtension(nameWithoutExtension);
			}
			strcat(nameWithoutExtension,"\0");
		} else {
			nameWithoutExtension=malloc(strlen(rom->name)+1);
			strcpy(nameWithoutExtension,rom->name);
			strcat(nameWithoutExtension,"\0");
			if(stripGames) {
				stripGameName(nameWithoutExtension);
			} else {
				char *tempGame=getNameWithoutPath(nameWithoutExtension);
				strcpy(tempGame,getNameWithoutExtension(tempGame));
				strcpy(nameWithoutExtension,tempGame);
				free(tempGame);
			}
		}
		if (fullscreenMode) {
			sprintf(buf,"%s", nameWithoutExtension);
		} else {
			sprintf(buf,"%s", nameWithoutExtension);
		}
		if (i==menuSections[currentSectionNumber].currentGameInPage) {
			if(strlen(buf)>1) {
				if(fullscreenMode) {
					if(!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
						drawShadedGameNameOnScreenPicMode(buf, nextLine);
					}
				} else {
					if(MENU_ITEMS_PER_PAGE==12) {
						int screenDivisions=(SCREEN_RATIO*5)/1.33;
						int romListWidth=SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)*2-calculateProportionalSizeOrDistance(2);
						MAGIC_NUMBER = romListWidth;
						drawShadedGameNameOnScreenCenter(buf, nextLine);
					} else if(MENU_ITEMS_PER_PAGE==16) {
						int screenDivisions=(SCREEN_RATIO*5)/1.33;
						int romListWidth=SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(43);
						MAGIC_NUMBER = romListWidth;
						drawShadedGameNameOnScreenLeft(buf, nextLine);
					} else {
						drawShadedGameNameOnScreen(buf, nextLine);
					}
				}
			}
		} else {
			if(strlen(buf)>1) {
				if(fullscreenMode) {
					if(!isPicModeMenuHidden&&menuVisibleInFullscreenMode) {
						drawNonShadedGameNameOnScreenPicMode(buf, nextLine);
					}
				} else {
					if(MENU_ITEMS_PER_PAGE==12) {
						int screenDivisions=(SCREEN_RATIO*5)/1.33;
						int romListWidth=SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)*2-calculateProportionalSizeOrDistance(2);
						MAGIC_NUMBER = romListWidth;
						drawNonShadedGameNameOnScreenCenter(buf, nextLine);
					} else if(MENU_ITEMS_PER_PAGE==16) {
						int screenDivisions=(SCREEN_RATIO*5)/1.33;
						int romListWidth=SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(43);
						MAGIC_NUMBER = romListWidth;
						drawNonShadedGameNameOnScreenLeft(buf, nextLine);
					} else {
						drawNonShadedGameNameOnScreen(buf, nextLine);
					}
				}
			}
		}
		if (!fullscreenMode) {
			nextLine+=calculateProportionalSizeOrDistance((fontSize*19)/14);
		} else {
			nextLine+=calculateProportionalSizeOrDistance((fontSize*20)/14);
		}
		free(nameWithoutExtension);
		currentNode = currentNode->next;
	}
	MAGIC_NUMBER = SCREEN_WIDTH-calculateProportionalSizeOrDistance(2);
}

void drawFooter(char *text) {
	int rgbColor[] = {menuSections[currentSectionNumber].headerAndFooterBackgroundColor[0],menuSections[currentSectionNumber].headerAndFooterBackgroundColor[1],menuSections[currentSectionNumber].headerAndFooterBackgroundColor[2]};
	if(ITEMS_PER_PAGE!=12) {
		drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance((22*fontSize)/14), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance((22*fontSize)/14), rgbColor);
	} else {
		drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), rgbColor);
	}
	drawTextOnFooter(text);
}
void setupDecorations(struct Rom *rom) {
	char tempString[200];
	if (rom==NULL||rom->name==NULL) {
		snprintf(tempString,sizeof(tempString),"GAME %d of %d",CURRENT_SECTION.currentGameInPage+ITEMS_PER_PAGE*CURRENT_SECTION.currentPage, CURRENT_SECTION.gameCount);
	} else {
		snprintf(tempString,sizeof(tempString),"GAME %d of %d",CURRENT_SECTION.currentGameInPage+1+ITEMS_PER_PAGE*CURRENT_SECTION.currentPage, CURRENT_SECTION.gameCount);
	}
	drawFooter(tempString);
	drawHeader(rom);
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
	options[ITEMS_PER_PAGE_OPTION]= malloc(100);

	values[TIDY_ROMS_OPTION]= malloc(4);
	values[FULL_SCREEN_FOOTER_OPTION]= malloc(4);
	values[FULL_SCREEN_MENU_OPTION]= malloc(4);
	values[THEME_OPTION]= malloc(2000);
	values[SCREEN_TIMEOUT_OPTION]= malloc(40);
	values[DEFAULT_OPTION]= malloc(4);
	values[USB_OPTION]= malloc(4);
	values[SHUTDOWN_OPTION]= malloc(4);
	values[AUTO_HIDE_LOGOS_OPTION]= malloc(4);
	values[ITEMS_PER_PAGE_OPTION]=malloc(30);

	hints[TIDY_ROMS_OPTION]= malloc(100);
	hints[FULL_SCREEN_FOOTER_OPTION]= malloc(100);
	hints[FULL_SCREEN_MENU_OPTION]= malloc(100);
	hints[THEME_OPTION]= malloc(100);
	hints[SCREEN_TIMEOUT_OPTION]= malloc(100);
	hints[DEFAULT_OPTION]= malloc(100);
	hints[USB_OPTION]= malloc(100);
	hints[SHUTDOWN_OPTION]= malloc(100);
	hints[AUTO_HIDE_LOGOS_OPTION]= malloc(100);
	hints[ITEMS_PER_PAGE_OPTION]= malloc(100);

	strcpy(options[TIDY_ROMS_OPTION],"Tidy rom names: ");
	strcpy(options[FULL_SCREEN_FOOTER_OPTION],"Display fullscreen footer: ");
	strcpy(options[FULL_SCREEN_MENU_OPTION],"Display fullscreen menu: ");
	strcpy(options[THEME_OPTION],"Theme: ");
	strcpy(options[SCREEN_TIMEOUT_OPTION],"Screen timeout: ");
	strcpy(options[DEFAULT_OPTION],"Default launcher: ");
	strcpy(options[USB_OPTION],"USB mode");
	strcpy(options[AUTO_HIDE_LOGOS_OPTION],"Auto-hide logos: ");
	strcpy(options[ITEMS_PER_PAGE_OPTION],"Layout: ");

	if (shutDownEnabled) {
		strcpy(options[SHUTDOWN_OPTION],"Shutdown");
	} else {
		strcpy(options[SHUTDOWN_OPTION],"Quit");
	}

	strcpy(hints[TIDY_ROMS_OPTION],"CUT DETAILS OUT OF ROM NAMES");
	strcpy(hints[FULL_SCREEN_FOOTER_OPTION],"DISPLAY THE CURRENT ROM NAME");
	strcpy(hints[FULL_SCREEN_MENU_OPTION],"DISPLAY A TRANSLUCENT MENU");
	strcpy(hints[THEME_OPTION],"LAUNCHER THEME");
	strcpy(hints[SCREEN_TIMEOUT_OPTION],"SECS UNTIL THE SCREEN TURNS OFF");
	strcpy(hints[DEFAULT_OPTION],"LAUNCH AFTER BOOTING");
	strcpy(hints[AUTO_HIDE_LOGOS_OPTION],"HIDE LOGOS AFTER A SECOND");
	strcpy(hints[ITEMS_PER_PAGE_OPTION],"LAYOUT TYPE");
	strcpy(hints[USB_OPTION],"PRESS A TO ENABLE USB");

	if (shutDownEnabled) {
		strcpy(hints[SHUTDOWN_OPTION],"PRESS A TO SHUTDOWN");
	} else {
		strcpy(hints[SHUTDOWN_OPTION],"PRESS A TO QUIT");
	}

	if (stripGames) {
		strcpy(values[TIDY_ROMS_OPTION],"YES");
	} else {
		strcpy(values[TIDY_ROMS_OPTION],"NO");
	}
	if (footerVisibleInFullscreenMode) {
		strcpy(values[FULL_SCREEN_FOOTER_OPTION],"YES");
	} else {
		strcpy(values[FULL_SCREEN_FOOTER_OPTION],"NO");
	}
	if (menuVisibleInFullscreenMode) {
		strcpy(values[FULL_SCREEN_MENU_OPTION],"YES");
	} else {
		strcpy(values[FULL_SCREEN_MENU_OPTION],"NO");
	}
	char *themeName=getNameWithoutPath((themes[activeTheme]));
	strcpy(values[THEME_OPTION],themeName);
	free(themeName);
	sprintf(values[SCREEN_TIMEOUT_OPTION],"%d",timeoutValue);
	if(MENU_ITEMS_PER_PAGE==10) {
		strcpy(values[ITEMS_PER_PAGE_OPTION],"SIMPLE MENU");
	} else if (MENU_ITEMS_PER_PAGE==12) {
		strcpy(values[ITEMS_PER_PAGE_OPTION],"TRADITIONAL");
	} else {
		strcpy(values[ITEMS_PER_PAGE_OPTION],"DRUNKEN MONKEY");
	}
	if (shutDownEnabled) {
		strcpy(values[DEFAULT_OPTION],"YES");
	} else {
		strcpy(values[DEFAULT_OPTION],"NO");
	}
	strcpy(values[SHUTDOWN_OPTION],"\0");
	strcpy(values[USB_OPTION],"\0");
	if (autoHideLogos) {
		strcpy(values[AUTO_HIDE_LOGOS_OPTION],"YES");
	} else {
		strcpy(values[AUTO_HIDE_LOGOS_OPTION],"NO");
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
	ITEMS_PER_PAGE_OPTION=7;
	DEFAULT_OPTION=8;
	USB_OPTION=9;

	int darkerAmber[3]={150,102,15};
	int brighterAmber[3]= {243,197,31};

	char *options[11];
	char *values[11];
	char *hints[11];

	setOptionsAndValues(options, values, hints);

	drawRectangleOnScreen(SCREEN_WIDTH, SCREEN_HEIGHT, 0,0, brighterAmber);
	drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, 0, darkerAmber);
	drawRectangleOnScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance(22), 0, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(22), darkerAmber);
	drawTextOnSettingsHeaderWithColor("SETTINGS",brighterAmber);
	int nextLine = calculateProportionalSizeOrDistance((14*29)/14);//CHANGE FIRST VALUE FOR FONT SIZE
	#ifdef TARGET_RG300
	for (int i=0;i<10;i++) {
	#else
	for (int i=0;i<9;i++) {
	#endif
		char temp[300];
		strcpy(temp,options[i]);
		if(strlen(values[i])>0) {
			strcat(temp,values[i]);
		}
		if(i==chosenSetting) {
			drawShadedSettingsOptionOnScreen(temp, nextLine,brighterAmber,darkerAmber);
			drawTextOnSettingsFooterWithColor(hints[i], brighterAmber);
		} else {
			drawNonShadedSettingsOptionOnScreen(temp, nextLine, darkerAmber);
		}
		nextLine+=calculateProportionalSizeOrDistance((14*19)/14);//CHANGE LAST VALUE FOR FONT SIZE
	}
	free(options[TIDY_ROMS_OPTION]);
	free(options[FULL_SCREEN_FOOTER_OPTION]);
	free(options[FULL_SCREEN_MENU_OPTION]);
	free(options[THEME_OPTION]);
	free(options[SCREEN_TIMEOUT_OPTION]);
	free(options[DEFAULT_OPTION]);
	free(options[USB_OPTION]);
	free(options[SHUTDOWN_OPTION]);
	free(options[ITEMS_PER_PAGE_OPTION]);

	free(values[TIDY_ROMS_OPTION]);
	free(values[FULL_SCREEN_FOOTER_OPTION]);
	free(values[FULL_SCREEN_MENU_OPTION]);
	free(values[THEME_OPTION]);
	free(values[SCREEN_TIMEOUT_OPTION]);
	free(values[DEFAULT_OPTION]);
	free(values[SHUTDOWN_OPTION]);
	free(values[USB_OPTION]);
	free(values[ITEMS_PER_PAGE_OPTION]);

	free(hints[TIDY_ROMS_OPTION]);
	free(hints[FULL_SCREEN_FOOTER_OPTION]);
	free(hints[FULL_SCREEN_MENU_OPTION]);
	free(hints[THEME_OPTION]);
	free(hints[SCREEN_TIMEOUT_OPTION]);
	free(hints[DEFAULT_OPTION]);
	free(hints[USB_OPTION]);
	free(hints[SHUTDOWN_OPTION]);
	free(hints[ITEMS_PER_PAGE_OPTION]);

}

void updateScreen(struct Rom *rom) {
	//    pthread_mutex_lock(&lock);
	if (!currentlySectionSwitching&&!isUSBMode&&!itsStoppedBecauseOfAnError) {
		setupDecorations(rom);
		if (fullscreenMode) {
			displayGamePicture(rom);
		}
		drawGameList();
		if (!fullscreenMode&&(MENU_ITEMS_PER_PAGE==12||MENU_ITEMS_PER_PAGE==16)) {
			displayGamePictureInMenu(rom);
		} else {
			displayHeart();
		}
		if (currentlyChoosing==3) {
			drawSettingsScreen();
		} else if (currentlyChoosing==2) {
			showCurrentGroup();
		} else if (currentlyChoosing==1) {
			showCurrentEmulator();
		}
		if (CURRENT_SECTION.alphabeticalPaging) {
			if (rom->name!=NULL) {
				showLetter(rom);
			}
		}
	} else if (isUSBMode) {
		drawUSBScreen();
	} else if (itsStoppedBecauseOfAnError) {
		showErrorMessage(errorMessage);
	} else if (currentlySectionSwitching||picModeHideLogoTimer!=NULL){
		displayBackgroundPicture();
		showConsole();
	}
	refreshScreen();
	//    pthread_mutex_unlock(&lock);
}

void setupDisplayAndKeys() {
	//	pumpEvents();
	initializeDisplay();
	initializeFonts();
	initializeKeys();
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
		if (CURRENT_SECTION.currentGameNode!=NULL) {
			updateScreen(CURRENT_SECTION.currentGameNode->data);
		} else {
			updateScreen(NULL);
		}
	}
	return 0;
}

void resetPicModeHideMenuTimer() {
	isPicModeMenuHidden=0;
	clearPicModeHideMenuTimer();
	picModeHideMenuTimer=SDL_AddTimer(0.6 * 1e3, hideFullScreenModeMenu, NULL);
}

void clearPicModeHideLogoTimer() {
	if (picModeHideLogoTimer != NULL) {
		SDL_RemoveTimer(picModeHideLogoTimer);
	}
	picModeHideLogoTimer = NULL;
}

uint32_t hidePicModeLogo() {
//	while(loading) {
//	}
	clearPicModeHideLogoTimer();
	currentlySectionSwitching=0;
	hotKeyPressed=0;
	if (CURRENT_SECTION.currentGameNode!=NULL) {
		updateScreen(CURRENT_SECTION.currentGameNode->data);
	} else {
		updateScreen(NULL);
	}
	return 0;
}

void resetPicModeHideLogoTimer() {
	clearPicModeHideLogoTimer();
	picModeHideLogoTimer=SDL_AddTimer(0.8 * 1e3, hidePicModeLogo, NULL);
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
	if (CURRENT_SECTION.currentGameNode!=NULL) {
		updateScreen(CURRENT_SECTION.currentGameNode->data);
	} else {
		updateScreen(NULL);
	}
	return 0;
}

void resetHideHeartTimer() {
	clearHideHeartTimer();
	hideHeartTimer=SDL_AddTimer(0.5 * 1e3, hideHeart, NULL);
	if (CURRENT_SECTION.currentGameNode!=NULL) {
		updateScreen(CURRENT_SECTION.currentGameNode->data);
	} else {
		updateScreen(NULL);
	}
}
