#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL_joystick.h>
#include <SDL/SDL_timer.h>
#include <unistd.h>

#include "../headers/string_utils.h"

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>
#include "../headers/screen.h"

#if defined TARGET_OD || defined TARGET_OD_BETA
#include <shake.h>
#endif

#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/SDL_rotozoom.h"
#include "../headers/logic.h"
#include "../headers/graphics.h"
#include "../headers/globals.h"
#include "../headers/utils.h"

TTF_Font *font = NULL;
TTF_Font *miniFont = NULL;
TTF_Font *picModeFont = NULL;
TTF_Font *BIGFont = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *customCountFont = NULL;
TTF_Font *footerFont = NULL;
TTF_Font *gameCountFontFont = NULL;

TTF_Font *outlineFont = NULL;
TTF_Font *outlineMiniFont = NULL;
TTF_Font *outlineHeaderFont = NULL;
TTF_Font *outlineCustomCountFont = NULL;
TTF_Font *outlineFooterFont = NULL;


TTF_Font *settingsfont = NULL;
TTF_Font *settingsHeaderFont = NULL;
TTF_Font *settingsFooterFont = NULL;
TTF_Font *settingsStatusFont = NULL;

TTF_Font *customHeaderFont = NULL;
TTF_Font *outlineCustomHeaderFont = NULL;

TTF_Font *getGameCountFont() {
	return gameCountFontFont;
}

TTF_Font *getFont() {
	return font;
}

TTF_Font *getSettingsFont() {
	return settingsfont;
}

TTF_Font *getFooterFont() {
	return footerFont;
}

TTF_Font *getBigFont() {
	char *akashi = "resources/akashi.ttf";
	BIGFont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(14+18));
	return BIGFont;
}

SDL_Color make_color(Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.unused = 1;
	return c;
}

int calculateProportionalSizeOrDistance(int number) {
	if(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)
		return ((float)SCREEN_HEIGHT*(float)number)/240;
	else {
		int result = (((SCREEN_HEIGHT-(SCREEN_HEIGHT*60/240))*number)/180);
//		printf("FGL: %d\n", result);
//		if (result==476) {
//			result=480;
//		}
		return result;
	}
}

int genericDrawTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int backgroundColor[], int shaded) {
	SDL_Surface *msg;
	SDL_Surface *msg1 = malloc(sizeof(msg));
	char *bufCopy=malloc(strlen(buf)+1);
	strcpy(bufCopy,buf);

	int len=strlen(buf);
	int width = MAGIC_NUMBER;

	int retW = 1;

	TTF_SizeText(font, (const char *) buf, &retW, NULL);

	while (retW>width) {
		bufCopy[len]='\0';
		char *bufCopy1=strdup(bufCopy);
		len--;
		TTF_SizeText(font, (const char *) bufCopy1, &retW, NULL);
		free(bufCopy1);
	}

	if (shaded) {
		if (currentState==BROWSING_GAME_LIST  && outline != NULL && fontOutline > 0) {
			msg1 = TTF_RenderText_Shaded(outline, bufCopy, make_color(50,50,50), make_color(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
			msg = TTF_RenderText_Solid(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
		} else {
			msg = TTF_RenderText_Shaded(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]), make_color(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
		}
	} else {
		if (currentState==BROWSING_GAME_LIST && outline != NULL && fontOutline > 0) {
			msg1 = TTF_RenderText_Blended(outline, bufCopy, make_color(50, 50, 50));
			msg = TTF_RenderText_Solid(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
		} else {
			msg = TTF_RenderText_Blended(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
		}
	}

	if (align & HAlignCenter) {
		x -= msg->w / 2;
	} else if (align & HAlignRight) {
		x -= msg->w;
	}
	if (align & VAlignMiddle) {
		y -= msg->h / 2;
	} else if (align & VAlignTop) {
		y -= msg->h;
	}
	SDL_Rect rect2;
	rect2.x = x;
	rect2.y = y;
	rect2.w = msg->w;
	rect2.h = msg->h;

	if(currentState==BROWSING_GAME_LIST && outline != NULL && fontOutline > 0) {
		SDL_Rect rect = {fontOutline, fontOutline, msg1->w, msg1->h};
		SDL_BlitSurface(msg, NULL, msg1, &rect);
		SDL_BlitSurface(msg1, NULL, screen, &rect2);
		SDL_FreeSurface(msg1);
	} else {
		SDL_BlitSurface(msg, NULL, screen, &rect2);
	}
	SDL_FreeSurface(msg);
	free(bufCopy);
	return 1;
}

void genericDrawMultiLineTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int maxWidth, int lineSeparation) {
	SDL_Surface *msg;
	char *bufCopy;
	char *wordsInBuf[500];
	char *ptr = NULL;

	bufCopy = strdup(buf);

	ptr = strtok(bufCopy," ");
	int wordCounter = -1;
	while(ptr!=NULL) {
		wordCounter++;
		wordsInBuf[wordCounter]=strdup(ptr);
		ptr = strtok(NULL," ");
	}
	free (bufCopy);
	int printCounter = 0;
	char *test=NULL;
	if(wordCounter>0) {
		while(printCounter<wordCounter) {
			test=malloc(500);
			strcpy(test,wordsInBuf[printCounter]);
			if (printCounter>0) {
				SDL_FreeSurface(msg);
			}
			msg = TTF_RenderText_Blended(font, test, make_color(txtColor[0], txtColor[1], txtColor[2]));
			while (msg->w<=maxWidth&&printCounter<wordCounter) {
				printCounter++;
				if (strcmp(wordsInBuf[printCounter],"-")!=0) {
					strcat(test," ");
					strcat(test,wordsInBuf[printCounter]);
					SDL_FreeSurface(msg);
					msg = TTF_RenderText_Blended(font, test, make_color(txtColor[0], txtColor[1], txtColor[2]));
				} else {
					printCounter++;
					break;
				}
			}
			if (msg->w>maxWidth) {
				test[strlen(test)-strlen(wordsInBuf[printCounter])]='\0';
			}
			int h = 0;
			int w = 0;
			TTF_SizeText(miniFont, test, &w, &h);
			genericDrawTextOnScreen(font,outline,x,y,test,txtColor,align,NULL,0);
			free(test);
			y+=calculateProportionalSizeOrDistance(lineSeparation);
		}
		if (printCounter==wordCounter) {
			y-=calculateProportionalSizeOrDistance(lineSeparation);
			if (msg->w>maxWidth) {
				genericDrawTextOnScreen(font,outline,x,y+calculateProportionalSizeOrDistance(lineSeparation),wordsInBuf[printCounter],txtColor,align,NULL,0);
			}
		}
		SDL_FreeSurface(msg);
		for (int i=0;i<=wordCounter;i++) {
			free(wordsInBuf[i]);
		}

	} else {
		msg = TTF_RenderText_Blended(font, buf, make_color(txtColor[0], txtColor[1], txtColor[2]));
		genericDrawTextOnScreen(font,outline,x,y,buf,txtColor,align,NULL,0);
		free(wordsInBuf[0]);
		SDL_FreeSurface(msg);
	}
}

int drawShadedTextOnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align, int backgroundColor[]) {
	return genericDrawTextOnScreen(font, outline, x, y, buf, txtColor, align, backgroundColor, 1);
}

int drawTextOnScreen(TTF_Font *pfont, TTF_Font *outline, int x, int y, char *buf, int txtColor[], int align) {
	if (pfont==NULL) {
		pfont = getFont();
	}
	return genericDrawTextOnScreen(pfont, outline, x, y, buf, txtColor, align, NULL, 0);
}

void drawCustomGameNameUnderPictureOnScreen(char *buf, int x, int y, int maxWidth) {
	genericDrawMultiLineTextOnScreen(miniFont, outlineMiniFont, x, y, buf, CURRENT_SECTION.pictureTextColor, VAlignBottom|HAlignCenter, maxWidth, artTextLineSeparation);
}

void drawCustomGameNumber(char *buf, int x, int y) {
	int hAlign = HAlignLeft;
	if(text2Alignment==1) {
		hAlign = HAlignCenter;
	} else if (text2Alignment==2) {
		hAlign = HAlignRight;
	}
	genericDrawTextOnScreen(customCountFont, outlineCustomCountFont, x, y, buf, CURRENT_SECTION.fullscreenMenuItemsColor, VAlignMiddle|hAlign, CURRENT_SECTION.fullScreenMenuBackgroundColor, 0);
}

void drawCustomText1OnScreen(TTF_Font *font, TTF_Font *outline, int x, int y, const char buf[300], int txtColor[], int align){
	SDL_Surface *msg;
	SDL_Surface *msg1;

	char *bufCopy=malloc(300);
	char *bufCopy1=malloc(300);
	strcpy(bufCopy,buf);
	strcpy(bufCopy1,buf);
	bufCopy1[1]='\0';
	msg = TTF_RenderText_Blended(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
	msg1 = TTF_RenderText_Blended(outline, bufCopy, make_color(50,50,50));
	if (align & HAlignCenter) {
		x -= msg->w / 2;
	} else if (align & HAlignRight) {
		x -= msg->w;
	}

	if (align & VAlignMiddle) {
		y -= msg->h / 2;
	} else if (align & VAlignTop) {
		y -= msg->h;
	}
	SDL_Rect rect2;
	rect2.x = x;
	rect2.y = y+120;
	rect2.w = msg->w;
	rect2.h = msg->h;

	SDL_Rect rect = {1, 1, msg1->w, msg1->h};
	SDL_BlitSurface(msg, NULL, msg1, &rect);
	SDL_BlitSurface(msg1, NULL, screen, &rect2);
	SDL_FreeSurface(msg1);

	SDL_FreeSurface(msg);
	free(bufCopy);
}

void drawShadedSettingsOptionValueOnScreen(char *option, char *value, int position, int txtColor[], int txtBackgroundColor[]) {
	int retW=1;
	int retW2=1;
	int retW3=3;
	TTF_SizeText(settingsfont, (const char *) option, &retW, NULL);
	TTF_SizeText(settingsfont, (const char *) " ", &retW2, NULL);
	TTF_SizeText(settingsfont, (const char *) value, &retW3, NULL);
	drawShadedTextOnScreen(settingsfont, NULL, SCREEN_WIDTH-calculateProportionalSizeOrDistance(5)-retW3, position, value, txtColor, VAlignBottom | HAlignLeft, txtBackgroundColor);
}

void drawSettingsOptionValueOnScreen(char *value, int position, int txtColor[]) {
	int retW3=3;
	TTF_SizeText(settingsfont, (const char *) value, &retW3, NULL);
	drawTextOnScreen(settingsfont, NULL, SCREEN_WIDTH-calculateProportionalSizeOrDistance(5)-retW3, position, value, txtColor, VAlignBottom | HAlignLeft);
}

void drawNonShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[]) {
	drawTextOnScreen(settingsfont, NULL, calculateProportionalSizeOrDistance(5), position, buf, txtColor, VAlignBottom | HAlignLeft);
}

void drawShadedGameNameOnScreen(char *buf, int position) {
	char *temp = malloc(strlen(buf)+2);
	if (currentCPU == OC_UC) {
		strcpy(temp,"-");
		strcat(temp,buf);
	} else 	if (currentCPU == OC_OC) {
		strcpy(temp,"+");
		strcat(temp,buf);
	} else {
		strcpy(temp,buf);
	}
	drawShadedTextOnScreen(font, outlineFont, SCREEN_WIDTH/2, position, temp, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
	free(temp);
}

void drawShadedGameNameOnScreenLeft(char *buf, int position) {
	char *temp = malloc(strlen(buf)+2);
	if (currentCPU == OC_UC) {
		strcpy(temp,"-");
		strcat(temp,buf);
	} else 	if (currentCPU == OC_OC) {
		strcpy(temp,"+");
		strcat(temp,buf);
	} else {
		strcpy(temp,buf);
	}
	drawShadedTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(3), position, temp, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignLeft, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
	free(temp);
}

void drawNonShadedGameNameOnScreenLeft(char *buf, int position) {
	drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(3), position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignLeft);
}

void drawShadedGameNameOnScreenCenter(char *buf, int position) {
	char *temp = malloc(strlen(buf)+2);
	if (currentCPU == OC_UC) {
		strcpy(temp,"-");
		strcat(temp,buf);
	} else 	if (currentCPU == OC_OC) {
		strcpy(temp,"+");
		strcat(temp,buf);
	} else {
		strcpy(temp,buf);
	}
	int screenDivisions=(SCREEN_RATIO*5)/1.33;
	int centerRomList = (SCREEN_WIDTH-2*(SCREEN_WIDTH/screenDivisions))/2;
	drawShadedTextOnScreen(font, outlineFont, centerRomList, position, temp, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
	free(temp);
}

void drawNonShadedGameNameOnScreenCenter(char *buf, int position) {
	int screenDivisions=(SCREEN_RATIO*5)/1.33;
	int centerRomList = (SCREEN_WIDTH-2*(SCREEN_WIDTH/screenDivisions))/2;
	drawTextOnScreen(font, outlineFont, centerRomList, position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignCenter);
}

void drawShadedGameNameOnScreenCustom(char *buf, int position){
	char *temp = malloc(strlen(buf)+2);
//	if (CURRENT_SECTION.currentGameNode->data->preferences.frequency == OC_UC) {
//		strcpy(temp,"-");
//		strcat(temp,buf);
//	} else 	if (CURRENT_SECTION.currentGameNode->data->preferences.frequency == OC_OC) {
//		strcpy(temp,"+");
//		strcat(temp,buf);
//	} else {
		strcpy(temp,buf);
//	}
	int hAlign = 0;
	if (gameListAlignment==0) {
		hAlign = HAlignLeft;
	} else if (gameListAlignment==1) {
		hAlign = HAlignCenter;
	} else {
		hAlign = HAlignRight;
	}
	if (transparentShading) {
		drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(gameListX), position, temp, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | hAlign);
	} else {
		drawShadedTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(gameListX), position, temp, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | hAlign, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
	}
	free(temp);
}

void drawNonShadedGameNameOnScreenCustom(char *buf, int position) {
	if (gameListAlignment == 0) {
		drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(gameListX), position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignLeft);
	} else if (gameListAlignment == 1) {
		drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(gameListX), position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignCenter);
	} else {
		drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(gameListX), position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignRight);
	}
}

void drawNonShadedGameNameOnScreen(char *buf, int position) {
	drawTextOnScreen(font, outlineFont, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].menuItemsFontColor, VAlignBottom | HAlignCenter);
}

void drawShadedGameNameOnScreenPicMode(char *buf, int position) {
	char *temp = malloc(strlen(buf)+2);
//	if (currentCPU == OC_UC) {
//		strcpy(temp,"-");
//		strcat(temp,buf);
//	} else 	if (currentCPU == OC_OC) {
//		strcpy(temp,"+");
//		strcat(temp,buf);
//	} else {
		strcpy(temp,buf);
//	}
	int color[3];
//	if (colorfulFullscreenMenu) {
//		color[0] = CURRENT_SECTION.fullscreenShadedMenuItemsColor[0];
//		color[1] = CURRENT_SECTION.fullscreenShadedMenuItemsColor[1];
//		color[2] = CURRENT_SECTION.fullscreenShadedMenuItemsColor[2];
//	} else {
		color[0] = 255;
		color[1] = 255;
		color[2] = 0;
//	}
	drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(5), position, temp, color, VAlignMiddle | HAlignLeft);
	TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
	free(temp);
}

void drawNonShadedGameNameOnScreenPicMode(char *buf, int position) {
	int color[3];
	if (colorfulFullscreenMenu) {
		color[0] = CURRENT_SECTION.fullscreenMenuItemsColor[0];
		color[1] = CURRENT_SECTION.fullscreenMenuItemsColor[1];
		color[2] = CURRENT_SECTION.fullscreenMenuItemsColor[2];
	} else {
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
	}
	drawTextOnScreen(font, outlineFont, calculateProportionalSizeOrDistance(5), position, buf, color, VAlignMiddle | HAlignLeft);
}

void drawPictureTextOnScreen(char *buf) {
	if(!footerVisibleInFullscreenMode||!isPicModeMenuHidden) {
		return;
	}
	int h = 0;
	TTF_SizeText(font, buf, NULL, &h);
	char *temp = malloc(strlen(buf)+2);
	if (CURRENT_SECTION.currentGameNode->data->preferences.frequency == OC_UC) {
		strcpy(temp,"-");
		strcat(temp,buf);
	} else 	if (CURRENT_SECTION.currentGameNode->data->preferences.frequency == OC_OC) {
		strcpy(temp,"+");
		strcat(temp,buf);
	} else {
		strcpy(temp,buf);
	}
	if(!favoritesSectionSelected) {
		if (colorfulFullscreenMenu) {
			drawTransparentRectangleToScreen(SCREEN_WIDTH, h+calculateProportionalSizeOrDistance(2), 0, footerOnTop?0:SCREEN_HEIGHT-(h+calculateProportionalSizeOrDistance(2)), CURRENT_SECTION.fullScreenMenuBackgroundColor, 180);
			drawTransparentRectangleToScreen(SCREEN_WIDTH, h+calculateProportionalSizeOrDistance(2), 0, footerOnTop?0:SCREEN_HEIGHT-(h+calculateProportionalSizeOrDistance(2)),(int[]){0,0,0}, 100);
			drawTextOnScreen(font, NULL, SCREEN_WIDTH/2, footerOnTop?calculateProportionalSizeOrDistance(2):SCREEN_HEIGHT-h, temp, CURRENT_SECTION.fullscreenMenuItemsColor, footerOnTop?VAlignBottom|HAlignCenter:VAlignBottom|HAlignCenter);
		} else {
			drawTransparentRectangleToScreen(SCREEN_WIDTH, h+calculateProportionalSizeOrDistance(2), 0, footerOnTop?0:SCREEN_HEIGHT-(h+calculateProportionalSizeOrDistance(2)), (int[]){0,0,0}, 180);
			drawTextOnScreen(font, NULL, SCREEN_WIDTH/2, footerOnTop?calculateProportionalSizeOrDistance(2):SCREEN_HEIGHT-h, temp, (int[]){255,255,255}, footerOnTop?VAlignBottom|HAlignCenter:VAlignBottom|HAlignCenter);
		}
	} else {
		drawTransparentRectangleToScreen(SCREEN_WIDTH, h+calculateProportionalSizeOrDistance(2), 0, footerOnTop?0:SCREEN_HEIGHT-(h+calculateProportionalSizeOrDistance(2)), (int[]){0,0,0}, 180);
		drawTextOnScreen(font, NULL, SCREEN_WIDTH/2, footerOnTop?calculateProportionalSizeOrDistance(2):SCREEN_HEIGHT-h, temp, (int[]){255,255,0}, footerOnTop?VAlignBottom|HAlignCenter:VAlignBottom|HAlignCenter);
	}
	free(temp);
}

void drawImgFallbackTextOnScreen(char *fallBackText) {
	if(!footerVisibleInFullscreenMode) {
		char *temp = malloc(strlen(fallBackText)+2);
		if (currentCPU == OC_UC) {
			strcpy(temp,"-");
			strcat(temp,fallBackText);
		} else 	if (currentCPU == OC_OC) {
			strcpy(temp,"+");
			strcat(temp,fallBackText);
		} else {
			strcpy(temp,fallBackText);
		}
		drawTextOnScreen(font, NULL, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(120), temp, CURRENT_SECTION.menuItemsFontColor, VAlignMiddle | HAlignCenter);
		free(temp);
	} else {
		drawPictureTextOnScreen(fallBackText);
	}
}


void drawTextOnFooterWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(footerFont, outlineFooterFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), text, txtColor, VAlignMiddle| HAlignCenter);
}

void drawTextOnSettingsFooterWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(settingsStatusFont, NULL, calculateProportionalSizeOrDistance(5), calculateProportionalSizeOrDistance(231), text, txtColor, VAlignMiddle| HAlignLeft);
}

void drawTextOnHeader() {
	int Halign = 0;
	switch (text1Alignment) {
	case 0:
		Halign = HAlignLeft;
		break;
	case 1:
		Halign = HAlignCenter;
		break;
	case 2:
		Halign = HAlignRight;
		break;
	}
	if (currentSectionNumber==favoritesSectionNumber) {
		genericDrawTextOnScreen(customHeaderFont, outlineCustomHeaderFont, calculateProportionalSizeOrDistance(text1X), calculateProportionalSizeOrDistance(text1Y), favorites[CURRENT_GAME_NUMBER].section, menuSections[currentSectionNumber].fullscreenMenuItemsColor, VAlignMiddle | Halign, CURRENT_SECTION.fullScreenMenuBackgroundColor, 0);
	} else {
		genericDrawTextOnScreen(customHeaderFont, outlineCustomHeaderFont, calculateProportionalSizeOrDistance(text1X), calculateProportionalSizeOrDistance(text1Y), strlen(menuSections[currentSectionNumber].fantasyName)>1?menuSections[currentSectionNumber].fantasyName:menuSections[currentSectionNumber].sectionName, menuSections[currentSectionNumber].fullscreenMenuItemsColor, VAlignMiddle | Halign, CURRENT_SECTION.fullScreenMenuBackgroundColor, 0);
	}
}

void drawTextOnHeaderWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(headerFont, outlineHeaderFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(13), text, txtColor, VAlignMiddle | HAlignCenter);
}

void drawTextOnSettingsHeaderWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(settingsStatusFont, NULL, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(13), text, txtColor, VAlignMiddle | HAlignCenter);
}

void drawTextOnSettingsHeaderLeftWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(settingsHeaderFont, NULL, calculateProportionalSizeOrDistance(5), calculateProportionalSizeOrDistance(24), text, txtColor, VAlignMiddle | HAlignLeft);
}

void drawTextOnSettingsHeaderRightWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(settingsStatusFont, NULL, SCREEN_WIDTH-calculateProportionalSizeOrDistance(5), calculateProportionalSizeOrDistance(13), text, txtColor, VAlignMiddle | HAlignRight);
}

void drawTextOnSettingsHeaderRightWithColor1(char *text, int x, int txtColor[]) {
	drawTextOnScreen(settingsStatusFont, NULL, x, calculateProportionalSizeOrDistance(33), text, txtColor, VAlignMiddle | HAlignRight);
}

void drawCurrentLetter(char *letter, int textColor[], int x, int y) {
	drawTextOnScreen(font, NULL, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
}

void drawBigWhiteText(char *text) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(BIGFont, NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text, white, VAlignMiddle | HAlignCenter);
}

void drawLoadingText() {
	int white[3]={255, 255, 255};
	drawTextOnScreen(settingsFooterFont, NULL, SCREEN_WIDTH-calculateProportionalSizeOrDistance(44), SCREEN_HEIGHT-calculateProportionalSizeOrDistance(8), "LOADING...", white, VAlignMiddle | HAlignCenter);
	refreshScreen();
}

void drawCopyingText(char* text) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(settingsFooterFont, NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text, white, VAlignMiddle | HAlignCenter);
}

void drawTimeOnFooter(char *text) {
	drawTextOnScreen(font, NULL, calculateProportionalSizeOrDistance(316), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].fullscreenMenuItemsColor, VAlignMiddle | HAlignRight);
}

void drawBatteryOnFooter(char *text) {
	drawTextOnScreen(font, NULL, calculateProportionalSizeOrDistance(4), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].fullscreenMenuItemsColor, VAlignMiddle | HAlignLeft);
}

void drawCurrentSectionGroup(char *groupName, int textColor[]) {
	drawTextOnScreen(BIGFont, NULL, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), groupName, textColor, VAlignMiddle | HAlignCenter);
}

void drawError(char *errorMessage, int textColor[]) {
	if(strchr(errorMessage,'-')==NULL) {
		drawTextOnScreen(settingsfont, NULL, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), errorMessage, textColor, VAlignMiddle | HAlignCenter);
	} else {
		char *line2 = strchr(errorMessage,'-');
		int index = (line2-errorMessage);
		line2++;
		char line1[200];
		strcpy(line1, errorMessage);
		line1[index]='\0';
		drawTextOnScreen(settingsfont, NULL, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)-calculateProportionalSizeOrDistance(12), line1, textColor, VAlignMiddle | HAlignCenter);
		drawTextOnScreen(settingsfont, NULL, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)+calculateProportionalSizeOrDistance(12), line2, textColor, VAlignMiddle | HAlignCenter);
	}
}

SDL_Rect drawRectangleToScreen(int width, int height, int x, int y, int rgbColor[]) {
	char temp[500];
	snprintf(temp,sizeof(temp),"%d - %d - %d - %d - {%d,%d,%d}", width, height, x, y, rgbColor[0], rgbColor[1], rgbColor[2]);
	logMessage("INFO","drawRectangleToScreen",temp);
	SDL_Rect rectangle;
	rectangle.w = width;
	rectangle.h = height;
	rectangle.x = x;
	rectangle.y = y;
	logMessage("INFO","drawRectangleToScreen","Filling");
	SDL_FillRect(screen, &rectangle, SDL_MapRGB(screen->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	return(rectangle);
}

SDL_Surface *loadImage (char *fileName) {
	SDL_Surface *img = NULL;
	SDL_Surface *_img = IMG_Load(fileName);
	if (_img!=NULL) {
		img = SDL_DisplayFormatAlpha(_img);
		SDL_FreeSurface(_img);
	}
	return(img);
}

void displayBackGroundImage(char *fileName, SDL_Surface *surface) {
	SDL_Surface *img = loadImage (fileName);
	int rgbColor[]={0, 0, 0};
	SDL_Rect bgrect = drawRectangleToScreen(img->w, img->h, (SCREEN_WIDTH/2)-(img->w/2),(SCREEN_HEIGHT/2)-(img->h/2), rgbColor);
	SDL_BlitSurface(img, NULL, surface, &bgrect);
	SDL_FreeSurface(img);
}

void drawTransparentRectangleToScreen(int w, int h, int x, int y, int rgbColor[], int opacity) {
	SDL_Surface *transparentrectangle;
	SDL_Rect rectangleOrig;
	SDL_Rect rectangleDest;
	rectangleOrig.w = w;
	rectangleOrig.h = h;
	rectangleOrig.x = 0;
	rectangleOrig.y = 0;
	rectangleDest.w = w;
	rectangleDest.h = h;
	rectangleDest.x = x;
	rectangleDest.y = y;
	transparentrectangle = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 16, 0, 0, 0, 0);
	SDL_FillRect(transparentrectangle, &rectangleOrig, SDL_MapRGB(transparentrectangle->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	SDL_SetAlpha(transparentrectangle, SDL_SRCALPHA, opacity);
	SDL_BlitSurface(transparentrectangle, &rectangleOrig, screen, &rectangleDest);
	SDL_FreeSurface(transparentrectangle);
}

int drawImage(SDL_Surface* display, SDL_Surface *image, int x, int y, int xx, int yy , const double newwidth, const double newheight, int transparent, int smoothing) {
	// Zoom function uses doubles for rates of scaling, rather than
	// exact size values. This is how we get around that:
	double zoomx = newwidth  / (float)image->w;
	double zoomy = newheight / (float)image->h;
	// This function assumes no smoothing, so that any colorkeys wont bleed.
	SDL_Surface* sized = NULL;
	if (((int)newwidth<(int)(image->w/2))&&(int)(image->w/2)%(int)newwidth==0) {
		zoomx = (float)image->w/newwidth;
		zoomy = (float)image->h/newheight;
		sized = shrinkSurface(image, zoomx, zoomy);
	} else {
		zoomx = newwidth  / (float)image->w;
		zoomy = newheight / (float)image->h;
		sized = zoomSurface(image, zoomx, zoomy, smoothing);
	}	// If the original had an alpha color key, give it to the new one.
	if( image->flags & SDL_SRCCOLORKEY ) {
		// Acquire the original Key
		Uint32 colorkey = image->format->colorkey;
		// Set to the new image
		SDL_SetColorKey( sized, SDL_SRCCOLORKEY, colorkey );
	}
	// The original picture is no longer needed.
	SDL_FreeSurface(image);
	// Set it instead to the new image.
	image =  sized;
	SDL_Rect src, dest;
	src.x = xx; src.y = yy; src.w = image->w; src.h = image->h; // size
	dest.x =  x; dest.y = y; dest.w = image->w; dest.h = image->h;
	if(transparent == 1 ) {
		//Set the color as transparent
		SDL_SetColorKey(image,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(image->format,0x0,0x0,0x0));
	}
	SDL_BlitSurface(image, &src, display, &dest);
	SDL_FreeSurface(image);
	return 1;
}

void displayImageOnScreenCustom(char *fileName) {
	SDL_Surface *screenshot = IMG_Load(fileName);

	if (screenshot!=NULL) {
		double w = screenshot->w;
		double h = screenshot->h;
		double ratio = 0;  // Used for aspect ratio
		int smoothing = 1;
		ratio = w / h;   // get ratio for scaling image
		h = calculateProportionalSizeOrDistance(artHeight);
		w = h*ratio;
		smoothing = 0;
		if (w!=calculateProportionalSizeOrDistance(artWidth)) {
			ratio = h / w;   // get ratio for scaling image
			w = calculateProportionalSizeOrDistance(artWidth);
			h = w*ratio;
			if (h>calculateProportionalSizeOrDistance(artHeight)) {
				ratio = w / h;   // get ratio for scaling image
				h = calculateProportionalSizeOrDistance(artHeight);
				w = h*ratio;
			}
			smoothing=1;
		}
		smoothing=0;
		int heartX = 0;
		int heartY = 0;

		if (showArt) {
			heartX = calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2;
			heartY = calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)/2;
		} else {
			heartX = SCREEN_WIDTH/2;
			heartY = SCREEN_HEIGHT/2;
		}
//		displayHeart(heartX, heartY);
		if (showArt) {
			drawImage(screen, screenshot, calculateProportionalSizeOrDistance(artX+(artWidth/2))-w/2, calculateProportionalSizeOrDistance(artY+(artHeight/2))-h/2, 0, 0, w, h, 0, smoothing);
		}
		displayHeart(heartX, heartY);
		if(artTextDistanceFromPicture>=0 && showArt) {
			char temp[500];
			snprintf(temp,sizeof(temp),"%d/%d", CURRENT_SECTION.realCurrentGameNumber, CURRENT_SECTION.gameCount);
			if (currentGameNameBeingDisplayed[0]=='+') {
				drawCustomGameNameUnderPictureOnScreen(currentGameNameBeingDisplayed+1, calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2, calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)+calculateProportionalSizeOrDistance(artTextDistanceFromPicture),calculateProportionalSizeOrDistance(artWidth));
			} else {
				drawCustomGameNameUnderPictureOnScreen(currentGameNameBeingDisplayed, calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2, calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)+calculateProportionalSizeOrDistance(artTextDistanceFromPicture),calculateProportionalSizeOrDistance(artWidth));
			}
		}
	} else {
		int heartX = 0;
		int heartY = 0;
		if (showArt) {
			heartX = calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2;
			heartY = calculateProportionalSizeOrDistance(artY)+(calculateProportionalSizeOrDistance((artWidth/4)*3)/2);
		} else {
			heartX = SCREEN_WIDTH/2;
			heartY = SCREEN_HEIGHT/2;
		}
//		int heartY = calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)/2;
		displayHeart(heartX, heartY);
		if(artTextDistanceFromPicture>=0 && showArt) {
			char temp[500];
			snprintf(temp,sizeof(temp),"%d/%d", CURRENT_SECTION.realCurrentGameNumber, CURRENT_SECTION.gameCount);
			int artHeight = (artWidth/4)*3;
			if (CURRENT_SECTION.gameCount>0) {
				if (currentGameNameBeingDisplayed[0]=='+') {
					drawCustomGameNameUnderPictureOnScreen(currentGameNameBeingDisplayed+1, calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2, calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)+calculateProportionalSizeOrDistance(artTextDistanceFromPicture),calculateProportionalSizeOrDistance(artWidth));
				} else {
					drawCustomGameNameUnderPictureOnScreen(currentGameNameBeingDisplayed, calculateProportionalSizeOrDistance(artX)+calculateProportionalSizeOrDistance(artWidth)/2, calculateProportionalSizeOrDistance(artY)+calculateProportionalSizeOrDistance(artHeight)+calculateProportionalSizeOrDistance(artTextDistanceFromPicture),calculateProportionalSizeOrDistance(artWidth));
				}
			}
		}
	}
	if(systemX>0&&systemY>0) {
		displaySurface(CURRENT_SECTION.systemPictureSurface,calculateProportionalSizeOrDistance(systemX), calculateProportionalSizeOrDistance(systemY));
	}
}

void displayHeart(int x, int y) {
	if(hideHeartTimer!=NULL) {
		SDL_Surface *heart = IMG_Load(favoriteIndicator);
		if (heart!=NULL) {
			double wh = heart->w;
			double hh = heart->h;
			double ratioh = 0;  // Used for aspect ratio
			int smoothing = 1;
			ratioh = wh / hh;   // get ratio for scaling image
			hh = calculateProportionalSizeOrDistance(heart->h);
			if(hh!=heart->h) {
				smoothing = 1;
			}
			wh = hh*ratioh;
			smoothing = 1;
			drawImage(screen, heart, x-(wh/2), y-(hh/2), 0, 0, wh, hh, 0, smoothing);
		}
	}
}

void* thread_func(void *picture) {
	// Zoom function uses doubles for rates of scaling, rather than
	// exact size values. This is how we get around that:
	double zoomx = ((threadPicture*)picture)->newwidth  / (float)((threadPicture*)picture)->image->w;
	double zoomy = ((threadPicture*)picture)->newheight / (float)((threadPicture*)picture)->image->h;
	// This function assumes no smoothing, so that any colorkeys wont bleed.
	SDL_Surface* sized = NULL;
	if (((int)((threadPicture*)picture)->newwidth<(int)(((threadPicture*)picture)->image->w/2))&&((int)(((threadPicture*)picture)->image->w/2)%(int)((threadPicture*)picture)->newwidth)==0) {
		zoomx = (float)((threadPicture*)picture)->image->w/((threadPicture*)picture)->newwidth;
		zoomy = (float)((threadPicture*)picture)->image->h/((threadPicture*)picture)->newheight;
		sized = shrinkSurface(((threadPicture*)picture)->image, zoomx, zoomy);
	} else {
		zoomx = ((threadPicture*)picture)->newwidth  / (float)((threadPicture*)picture)->image->w;
		zoomy = ((threadPicture*)picture)->newheight / (float)((threadPicture*)picture)->image->h;
		sized = zoomSurface(((threadPicture*)picture)->image, zoomx, zoomy, ((threadPicture*)picture)->smoothing);
	}
	// If the original had an alpha color key, give it to the new one.
	if(((threadPicture*)picture)->image->flags & SDL_SRCCOLORKEY ) {
		// Acquire the original Key
		Uint32 colorkey = ((threadPicture*)picture)->image->format->colorkey;
		// Set to the new image
		SDL_SetColorKey( sized, SDL_SRCCOLORKEY, colorkey );
	}
	// The original picture is no longer needed.
	SDL_FreeSurface(((threadPicture*)picture)->image);
	// Set it instead to the new image.
	((threadPicture*)picture)->image =  sized;
	SDL_Rect src, dest;
	src.x = ((threadPicture*)picture)->xx; src.y = ((threadPicture*)picture)->yy; src.w = ((threadPicture*)picture)->image->w; src.h = ((threadPicture*)picture)->image->h; // size
	dest.x =  ((threadPicture*)picture)->x; dest.y = ((threadPicture*)picture)->y; dest.w = ((threadPicture*)picture)->image->w; dest.h = ((threadPicture*)picture)->image->h;
	if(((threadPicture*)picture)->transparent == 1 ) {
		//Set the color as transparent
		SDL_SetColorKey(((threadPicture*)picture)->image,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(((threadPicture*)picture)->image->format,0x0,0x0,0x0));
	}
	SDL_BlitSurface(((threadPicture*)picture)->image, &src, ((threadPicture*)picture)->display, &dest);
	SDL_FreeSurface(((threadPicture*)picture)->image);

	pthread_exit(NULL); // you could also return NULL here to exit no difference
	return (int*)1;
}

SDL_Surface *resizeSurfaceToFitScreen (SDL_Surface *surface) {
	if (surface==NULL) {
		logMessage("WARN","resizeSurfaceToFitScreen","Image not found, surface can't be resized");
		return NULL;
	}
	if (SCREEN_WIDTH==surface->w&&SCREEN_HEIGHT==surface->h) {
		return surface;
	}
	int smoothing = 0;
	if ((surface->w!=SCREEN_WIDTH || surface->h!=SCREEN_HEIGHT) && !(SCREEN_WIDTH%surface->w==0 && SCREEN_HEIGHT%surface->h==0)) {
		smoothing=1;
	}
	double zoomx = (float)SCREEN_WIDTH / surface->w;
	double zoomy = (float)SCREEN_HEIGHT / surface->h;
	SDL_Surface *sized = NULL;
	sized = zoomSurface(surface, zoomx, zoomy, smoothing);
	if(surface->flags & SDL_SRCCOLORKEY ) {
		Uint32 colorkey = surface->format->colorkey;
		SDL_SetColorKey(sized, SDL_SRCCOLORKEY, colorkey);
	}
	printf("asdasdas%d\n",sized->h);
	if(sized->h==(SCREEN_HEIGHT-1)) {
		sized->h+=1;
	}
	free(surface);
	return sized;
}

SDL_Surface *resizeSurfaceToScreenSize(SDL_Surface *surface) {
	if (surface==NULL) {
		logMessage("WARN","resizeSurface","Image not found, surface can't be resized");
		return NULL;
	}
	int newW = SCREEN_WIDTH;
	int newH = SCREEN_HEIGHT;
	if (newW==surface->w&&newH==surface->h) {
		return surface;
	}
	int smoothing = 1;
//	if ((surface->w!=calculateProportionalSizeOrDistance(w) || surface->h!=calculateProportionalSizeOrDistance(h)) && !(calculateProportionalSizeOrDistance(w)%surface->w==0 && calculateProportionalSizeOrDistance(h)%surface->h==0)) {
//		smoothing=1;
//	}
	double zoomx = (double)(newW / (double)surface->w);
	double zoomy = (double)(newH / (double)surface->h);

	SDL_Surface *sized = NULL;
	sized = zoomSurface(surface, zoomx, zoomy, smoothing);
	if(surface->flags & SDL_SRCCOLORKEY ) {
		Uint32 colorkey = surface->format->colorkey;
		SDL_SetColorKey(sized, SDL_SRCCOLORKEY, colorkey);
	}
	SDL_FreeSurface(surface);
	surface=NULL;
	return sized;
}

SDL_Surface *resizeSurface(SDL_Surface *surface, int w, int h) {
	if (surface==NULL) {
		logMessage("WARN","resizeSurface","Image not found, surface can't be resized");
		return NULL;
	}
	int newW = (float)calculateProportionalSizeOrDistance(w);
	int newH = (float)calculateProportionalSizeOrDistance(h);
	if (newW==surface->w&&newH==surface->h) {
		return surface;
	}
	int smoothing = 0;
	if ((surface->w!=calculateProportionalSizeOrDistance(w) || surface->h!=calculateProportionalSizeOrDistance(h)) && !(calculateProportionalSizeOrDistance(w)%surface->w==0 && calculateProportionalSizeOrDistance(h)%surface->h==0)) {
		smoothing=1;
	}
	double zoomx = (float)(newW / (float)surface->w);
	double zoomy = (float)(newH / (float)surface->h);

	SDL_Surface *sized = NULL;
	sized = zoomSurface(surface, zoomx, zoomy, smoothing);
	if(surface->flags & SDL_SRCCOLORKEY ) {
		Uint32 colorkey = surface->format->colorkey;
		SDL_SetColorKey(sized, SDL_SRCCOLORKEY, colorkey);
	}
	SDL_FreeSurface(surface);
	surface=NULL;
	return sized;
}


void resizeSectionSystemLogo(struct MenuSection *section) {
	section->systemLogoSurface = resizeSurfaceToScreenSize(section->systemLogoSurface);
}

void resizeSectionBackground(struct MenuSection *section) {
	section->backgroundSurface = resizeSurfaceToScreenSize(section->backgroundSurface);
}

void resizeGroupBackground(struct SectionGroup *group) {
	group->groupBackgroundSurface = resizeSurfaceToScreenSize(group->groupBackgroundSurface);
}

void resizeSectionSystemPicture(struct MenuSection *section) {
	section->systemPictureSurface = resizeSurface(section->systemPictureSurface, systemWidth, systemHeight);
}

void displayCenteredSurface(SDL_Surface *surface) {
	if(surface==NULL) {
		logMessage("WARN","displayCenteredSurface","Image not found, surface can't be displayed");
		return;
	}
	drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0,0,(int[]){180,180,180});
	SDL_Rect rectangleDest;
	rectangleDest.w = 0;
	rectangleDest.h = 0;
	rectangleDest.x = SCREEN_WIDTH/2-surface->w/2;
	rectangleDest.y = ((SCREEN_HEIGHT)/2-surface->h/2);
	SDL_BlitSurface(surface, NULL, screen, &rectangleDest);
	logMessage("INFO","displayCenteredSurface","Displayed surface");
}

void displaySurface(SDL_Surface *surface, int x, int y) {
	if(surface==NULL) {
		logMessage("WARN","displaySurface","Image not found, surface can't be displayed");
		return;
	}
	SDL_Rect rectangleDest;
	rectangleDest.w = 0;
	rectangleDest.h = 0;
	rectangleDest.x = x;
	rectangleDest.y = y;
	SDL_BlitSurface(surface, NULL, screen, &rectangleDest);
}

void displayCenteredImageOnScreen(char *fileName, char *fallBackText, int scaleToFullScreen, int keepRatio) {
	SDL_Surface *img = IMG_Load(fileName);
	if (img==NULL) {
		if (strlen(fallBackText)>1) {
			drawImgFallbackTextOnScreen(fallBackText);
		}
	} else {
		if (!scaleToFullScreen&&(img->h==SCREEN_HEIGHT || img->w==SCREEN_WIDTH)) {
			SDL_Rect rectangleDest;
			rectangleDest.w = 0;
			rectangleDest.h = 0;
			rectangleDest.x = SCREEN_WIDTH/2-img->w/2;
			rectangleDest.y = ((SCREEN_HEIGHT)/2-img->h/2);
			SDL_BlitSurface(img, NULL, screen, &rectangleDest);
			SDL_FreeSurface(img);
		} else {
			if(img->h==SCREEN_HEIGHT && img->w==SCREEN_WIDTH) {
				SDL_Rect rectangleDest;
				rectangleDest.w = 0;
				rectangleDest.h = 0;
				rectangleDest.x = SCREEN_WIDTH/2-img->w/2;
				rectangleDest.y = ((SCREEN_HEIGHT)/2-img->h/2);
				SDL_BlitSurface(img, NULL, screen, &rectangleDest);
				SDL_FreeSurface(img);
			} else {
				double w = img->w;
				double h = img->h;
				double ratio = 0;  // Used for aspect ratio
				int smoothing = 0;
				ratio = w / h;   // get ratio for scaling image
				h = SCREEN_HEIGHT;
				w = h*ratio;
				if (w>SCREEN_WIDTH) {
					ratio = h / w;   // get ratio for scaling image
					w = SCREEN_WIDTH;
					h = w*ratio;
				}
				if (!keepRatio) {
					w=SCREEN_WIDTH;
				}
				if ((int)h!=(int)img->h) {
					smoothing = 1;
				}
				drawImage(screen, img, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2-h/2, 0, 0, w, h, 0, smoothing);
			}
		}
	}
}

void drawUSBScreen() {
	int white[3]={255, 255, 255};
	int black[3]={0, 0, 0};
	displayCenteredImageOnScreen("./usb.png"," ",1,0);
	drawTextOnScreen(headerFont, NULL, 163,27,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont, NULL, 163,29,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont, NULL, 161,27,"USB MODE",white,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont, NULL, 163,217,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont, NULL, 163,219,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont, NULL, 161,217,"PRESS START TO END",white,VAlignMiddle | HAlignCenter);
}

void initializeDisplay() {
	SDL_ShowCursor(0);
	logMessage("INFO","initializeDisplay","well...");
	setenv("SDL_FBCON_DONT_CLEAR", "1", 0);
	logMessage("INFO","initializeDisplay","maybe...");
#ifdef TARGET_OD
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE);
	SDL_FreeSurface(screen);
	SDL_Quit();
#endif
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);


//#ifdef TARGET_BITTBOY
//	logMessage("INFO","yes?");
//	logMessage("INFO","333");
//	SCREEN_WIDTH=320;
//	SCREEN_HEIGHT=240;
//	logMessage("INFO","444");
//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_NOFRAME|SDL_SWSURFACE);
//	logMessage("INFO","555");
//	//	TTF_Init();
//	MAGIC_NUMBER = SCREEN_WIDTH-calculateProportionalSizeOrDistance(2);
//	logMessage("INFO","Initialized Display");
//	SCREEN_RATIO = (double)SCREEN_WIDTH/SCREEN_HEIGHT;
//	return;
//#endif

	//    SDL_JoystickEventState(SDL_ENABLE);
	//    joystick = SDL_JoystickOpen(0);

	char * line = NULL;
	size_t len = 0;
	FILE *fpHDMI = fopen("/sys/class/hdmi/hdmi","r");
	ssize_t read;
	if (fpHDMI!=NULL) {
		read = getline(&line, &len, fpHDMI);
		hdmiEnabled = atoi(line);
		fclose(fpHDMI);
		if (read!=-1) {
			free(line);
		}
	}

	hdmiChanged = hdmiEnabled;
	if (hdmiEnabled) {
		SCREEN_WIDTH = HDMI_WIDTH;
		SCREEN_HEIGHT = HDMI_HEIGHT;
	}
	SCREEN_RATIO = (double)SCREEN_WIDTH/SCREEN_HEIGHT;


#ifdef TARGET_RFW
	//	ipu modes (/proc/jz/ipu):
	//	0: stretch
	//	1: aspect
	//	2: original (fallback to aspect when downscale is needed)
	//	3: 4:3
	FILE *fp;
	fp = fopen("/proc/jz/ipu","w");
	fprintf(fp,"0");
	fclose(fp);
#endif
#ifdef TARGET_PC
//	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
	//	SCREEN_HEIGHT = info->current_h;
	SCREEN_HEIGHT = 240;
	SCREEN_WIDTH = (SCREEN_HEIGHT/3)*4;
	char msg[1000];
	snprintf(msg,1000,"%dx%d",SCREEN_WIDTH,SCREEN_HEIGHT);
	logMessage("INFO", "initializeDisplay", msg);
	SCREEN_RATIO = (double)SCREEN_WIDTH/SCREEN_HEIGHT;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_NOFRAME);
//	SDL_ShowCursor(0);
#else
	FILE *fp1;
	SDL_ShowCursor(0);

	SCREEN_WIDTH=320;
	SCREEN_HEIGHT=240;

	SDL_Rect** modes = SDL_ListModes(NULL,SDL_NOFRAME|SDL_SWSURFACE);

	fp1 = fopen("/sys/class/graphics/fb0/device/allow_downscaling","w");
	if (fp1!=NULL) {
		fprintf(fp1, "%d" , 0);
		fclose(fp1);
	}

#if defined TARGET_OD || defined TARGET_OD_BETA
	if(modes==(SDL_Rect **)0) {
		printf("No available modes\n");
	} else if(modes==(SDL_Rect **)-1) {
		printf("All modes available\n");
	} else {
		printf("Available modes:\n");
		for(int i=0; modes[i]; i++) {
			printf("%dx%d\n", modes[i]->w, modes[i]->h);
			if (modes[i]->w==640 && modes[i]->h ==480) {
				SCREEN_WIDTH=640;
				SCREEN_HEIGHT=480;
				break;
			}
		}
	}
#endif
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_NOFRAME|SDL_SWSURFACE);
	if (screen==NULL) {
		SCREEN_WIDTH=320;
		SCREEN_HEIGHT=240;
		screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_NOFRAME|SDL_SWSURFACE);
	}

#endif
	//	TTF_Init();
	MAGIC_NUMBER = SCREEN_WIDTH-calculateProportionalSizeOrDistance(2);
	logMessage("INFO","initializeDisplay","Initialized Display");
	SCREEN_RATIO = (double)SCREEN_WIDTH/SCREEN_HEIGHT;
}



void refreshScreen() {
	SDL_Flip(screen);
//	int black[3] = {0,0,0};
//	drawRectangleToScreen(SCREEN_WIDTH,SCREEN_HEIGHT,0,0,black);
}

void initializeSettingsFonts() {
	logMessage("INFO","initializeSettingsFonts","Initializing Settings Fonts");
	char *akashi = "resources/akashi.ttf";
	settingsfont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(14));
	settingsHeaderFont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(27));
	settingsStatusFont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(14));
	settingsFooterFont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(15));
	logMessage("INFO","initializeSettingsFonts","Settings Fonts initialized");
}

void initializeFonts() {
	TTF_Init();
	char *akashi = "resources/akashi.ttf";

	font = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize));
	outlineFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize));;

	miniFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(artTextFontSize));
	outlineMiniFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(artTextFontSize));

	picModeFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+5));
	BIGFont = TTF_OpenFont(akashi, calculateProportionalSizeOrDistance(fontSize+18));
	headerFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+6));
	outlineHeaderFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+6));

	footerFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+2));
	outlineFooterFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+2));

	customHeaderFont = TTF_OpenFont(textXFont, calculateProportionalSizeOrDistance(text1FontSize));
	outlineCustomHeaderFont = TTF_OpenFont(textXFont, calculateProportionalSizeOrDistance(text1FontSize));

	customCountFont = TTF_OpenFont(textXFont, calculateProportionalSizeOrDistance(text2FontSize));
	outlineCustomCountFont = TTF_OpenFont(textXFont, calculateProportionalSizeOrDistance(text2FontSize));
	if (menuFont!=NULL && strlen(menuFont)>2) {
		TTF_SetFontOutline(outlineFont,fontOutline);
		TTF_SetFontOutline(outlineMiniFont,fontOutline);
		TTF_SetFontOutline(outlineHeaderFont,fontOutline);
		TTF_SetFontOutline(outlineFooterFont,fontOutline);
		TTF_SetFontOutline(outlineCustomHeaderFont,fontOutline);
		TTF_SetFontOutline(outlineCustomCountFont,fontOutline);
	}

	gameCountFontFont = TTF_OpenFont(gameCountFont, calculateProportionalSizeOrDistance(gameCountFontSize));

	logMessage("INFO","initializeFonts","Fonts initialized");
}

void freeFonts() {
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(customHeaderFont);
	customHeaderFont = NULL;
	TTF_CloseFont(customCountFont);
	customCountFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	TTF_CloseFont(picModeFont);
	picModeFont = NULL;
	TTF_CloseFont(miniFont);
	miniFont = NULL;
	TTF_CloseFont(BIGFont);
	BIGFont = NULL;
	TTF_CloseFont(outlineCustomHeaderFont);
	outlineCustomHeaderFont = NULL;
	//	TTF_CloseFont(outlineFont);
	//	outlineFont = NULL;
	TTF_CloseFont(outlineMiniFont);
	outlineMiniFont = NULL;
	TTF_CloseFont(outlineHeaderFont);
	outlineHeaderFont = NULL;
	TTF_CloseFont(outlineCustomCountFont);
	outlineCustomCountFont = NULL;
	TTF_CloseFont(outlineFooterFont);
	outlineFooterFont = NULL;
	TTF_CloseFont(gameCountFontFont);
	gameCountFontFont = NULL;
}

void freeSettingsFonts() {
	TTF_CloseFont(settingsfont);
	settingsfont = NULL;
	TTF_CloseFont(settingsHeaderFont);
	settingsHeaderFont = NULL;
	TTF_CloseFont(settingsFooterFont);
	settingsFooterFont = NULL;
	TTF_CloseFont(settingsStatusFont);
	settingsStatusFont = NULL;
}

void freeResources() {
	freeFonts();
	freeSettingsFonts();
	TTF_Quit();
#if defined TARGET_OD || defined TARGET_OD_BETA
	Shake_Stop(device, effect_id);
	Shake_EraseEffect(device, effect_id);
	Shake_Close(device);
	Shake_Quit();
#endif
#ifndef TARGET_PC
	closeLogFile();
#endif
	SDL_Quit();
}
