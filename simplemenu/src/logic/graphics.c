#include "../headers/graphics.h"
#include "../headers/globals.h"

#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_stdinc.h>
#include <SDL/SDL_video.h>

#ifdef TARGET_RG350
#include <shake.h>
#endif

#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/SDL_rotozoom.h"
#include "../headers/logic.h"

SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
TTF_Font *miniFont = NULL;
TTF_Font *picModeFont = NULL;
TTF_Font *BIGFont = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

TTF_Font *settingsfont = NULL;
TTF_Font *settingsHeaderFont = NULL;
TTF_Font *settingsFooterFont = NULL;

TTF_Font *customHeaderFont = NULL;

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
		return (SCREEN_HEIGHT*number)/240;
	else {
		return ((SCREEN_HEIGHT-(SCREEN_HEIGHT*60/240))*number)/180;
	}
}

int calculateProportionalHorizontalSizeOrDistance(int number) {
	if(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)
		return (SCREEN_WIDTH*number)/320;
	else {
		return ((SCREEN_WIDTH-(SCREEN_WIDTH*640/1920))*number)/1280;
	}
}



int genericDrawTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align, int backgroundColor[], int shaded) {
	SDL_Surface *msg;
	char *bufCopy=malloc(300);
	char *bufCopy1=malloc(300);
	strcpy(bufCopy,buf);
	strcpy(bufCopy1,buf);
	bufCopy1[1]='\0';
	if (shaded) {
		msg = TTF_RenderText_Shaded(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]), make_color(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
	} else {
		msg = TTF_RenderText_Blended(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
	}
	int len=strlen(buf);
	int width = MAGIC_NUMBER;
	while (msg->w>width) {
		bufCopy[len]='\0';
		SDL_FreeSurface(msg);
		if (shaded) {
			msg = TTF_RenderText_Shaded(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]), make_color(backgroundColor[0], backgroundColor[1], backgroundColor[2]));
		} else {
			msg = TTF_RenderText_Blended(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
		}
		len--;
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
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = msg->w;
	rect.h = msg->h;
	SDL_BlitSurface(msg, NULL, screen, &rect);
	SDL_FreeSurface(msg);
	free(bufCopy);
	return msg->w;
}

void genericDrawMultiLineTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align, int maxWidth, int lineSeparation) {
	SDL_Surface *msg;
	char *bufCopy=malloc(300);
	char *wordsInBuf[25];
	char *ptr = NULL;

	bufCopy=strdup(buf);

	ptr = strtok(bufCopy," ");
	int wordCounter = -1;
	while(ptr!=NULL) {
		wordCounter++;
		wordsInBuf[wordCounter]=malloc(strlen(ptr)+1);
		strcpy(wordsInBuf[wordCounter],ptr);
		ptr = strtok(NULL," ");
	}

	int printCounter = 0;
	char *test=NULL;
	if(wordCounter>0) {
		while(printCounter<wordCounter) {
			test=malloc(500);
			strcpy(test,wordsInBuf[printCounter]);
			msg = TTF_RenderText_Blended(font, test, make_color(txtColor[0], txtColor[1], txtColor[2]));
			while (msg->w<=maxWidth&&printCounter<wordCounter) {
				printCounter++;
				if (strcmp(wordsInBuf[printCounter],"-")!=0) {
					strcat(test," ");
					strcat(test,wordsInBuf[printCounter]);
					msg = TTF_RenderText_Blended(font, test, make_color(txtColor[0], txtColor[1], txtColor[2]));
				} else {
					printCounter++;
					break;
				}
			}
			if (msg->w>maxWidth) {
				test[strlen(test)-strlen(wordsInBuf[printCounter])]='\0';
			}
			//		drawTransparentRectangleToScreen(maxWidth,fontSize,x-x+2,y-1,(int[]){0,0,0},111);
			genericDrawTextOnScreen(font,x,y,test,txtColor,align,NULL,0);
			free(test);
			y+=calculateProportionalSizeOrDistance(lineSeparation);
		}
		if (printCounter==wordCounter) {
			y-=calculateProportionalSizeOrDistance(lineSeparation);
			if (msg->w>maxWidth) {
				genericDrawTextOnScreen(font,x,y+calculateProportionalSizeOrDistance(lineSeparation),wordsInBuf[printCounter],txtColor,align,NULL,0);
			}
		}
	} else {
		msg = TTF_RenderText_Blended(font, buf, make_color(txtColor[0], txtColor[1], txtColor[2]));
		genericDrawTextOnScreen(font,x,y,buf,txtColor,align,NULL,0);
	}

}

int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align, int backgroundColor[]) {
	return genericDrawTextOnScreen(font, x, y, buf, txtColor, align, backgroundColor, 1);
}

int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align) {
	return genericDrawTextOnScreen(font, x, y, buf, txtColor, align, NULL, 0);
}

void drawCustomGameNameUnderPictureOnScreen(const char buf[300], int x, int y, int maxWidth) {
	genericDrawMultiLineTextOnScreen(miniFont, x, y, buf, CURRENT_SECTION.bodyTextColor, HAlignCenter, maxWidth, artTextLineSeparationInCustom);
}

void drawCustomGameNameUnderPictureOnScreen1(const char buf[300], int x, int y, int maxWidth) {
	drawTransparentRectangleToScreen(38,15,121, 25,(int[]){0,0,0},180);
	genericDrawMultiLineTextOnScreen(miniFont, x, y, buf, (int[]){0,0,0}, HAlignCenter, maxWidth, artTextLineSeparationInCustom);
}


void drawCustomText1OnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align){
	SDL_Surface *msg;
	char *bufCopy=malloc(300);
	char *bufCopy1=malloc(300);
	strcpy(bufCopy,buf);
	strcpy(bufCopy1,buf);
	bufCopy1[1]='\0';
	msg = TTF_RenderText_Blended(font, bufCopy, make_color(txtColor[0], txtColor[1], txtColor[2]));
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
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = msg->w;
	rect.h = msg->h;
	SDL_BlitSurface(msg, NULL, screen, &rect);
	SDL_FreeSurface(msg);
	free(bufCopy);
}

void drawShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[], int txtBackgroundColor[]) {
	drawShadedTextOnScreen(settingsfont, SCREEN_WIDTH/2, position, buf, txtColor, VAlignBottom | HAlignCenter, txtBackgroundColor);
}

void drawNonShadedSettingsOptionOnScreen(char *buf, int position, int txtColor[]) {
	drawTextOnScreen(settingsfont, SCREEN_WIDTH/2, position, buf, txtColor, VAlignBottom | HAlignCenter);
}

void drawShadedGameNameOnScreen(char *buf, int position) {
	drawShadedTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
}

void drawShadedGameNameOnScreenLeft(char *buf, int position) {
	drawShadedTextOnScreen(font, calculateProportionalSizeOrDistance(3), position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignLeft, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
}

void drawNonShadedGameNameOnScreenLeft(char *buf, int position) {
	drawTextOnScreen(font, calculateProportionalSizeOrDistance(3), position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignLeft);
}

void drawShadedGameNameOnScreenCenter(char *buf, int position) {
	int screenDivisions=(SCREEN_RATIO*5)/1.33;
	int centerRomList = (SCREEN_WIDTH-2*(SCREEN_WIDTH/screenDivisions))/2;
	drawShadedTextOnScreen(font, centerRomList, position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
}

void drawNonShadedGameNameOnScreenCenter(char *buf, int position) {
	int screenDivisions=(SCREEN_RATIO*5)/1.33;
	int centerRomList = (SCREEN_WIDTH-2*(SCREEN_WIDTH/screenDivisions))/2;
	drawTextOnScreen(font, centerRomList, position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignCenter);
}

void drawShadedGameNameOnScreenPicMode(char *buf, int position) {
	//	drawShadedTextOnScreen(picModeFont, SCREEN_WIDTH/2, position, buf, make_color(0,0,0), VAlignBottom | HAlignCenter, make_color(255,255,255));
	int color[3] = {255,255,0};
	if (favoritesSectionSelected) {
		color[0]= 0;
		color[1]= 100;
		color[2]= 255;
	}
	//	TTF_SetFontStyle(font,TTF_STYLE_BOLD);
	drawTextOnScreen(font, calculateProportionalSizeOrDistance(5), position, buf, color, VAlignMiddle | HAlignLeft);
	TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
}

void drawShadedGameNameOnScreenCustom(char *buf, int position){
	int hAlign = 0;
	if (gameListAlignmentInCustom==0) {
		hAlign = HAlignLeft;
	} else if (gameListAlignmentInCustom==1) {
		hAlign = HAlignCenter;
	} else {
		hAlign = HAlignRight;
	}
	if (transparentShading) {
		drawTextOnScreen(font, calculateProportionalSizeOrDistance(gameListXInCustom), position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | hAlign);
	} else {
		drawShadedTextOnScreen(font, calculateProportionalSizeOrDistance(gameListXInCustom), position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | hAlign, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
	}
}

void drawNonShadedGameNameOnScreenCustom(char *buf, int position) {
	if (gameListAlignmentInCustom == 0) {
		drawTextOnScreen(font, calculateProportionalSizeOrDistance(gameListXInCustom), position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignLeft);
	} else if (gameListAlignmentInCustom == 1) {
		drawTextOnScreen(font, calculateProportionalSizeOrDistance(gameListXInCustom), position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignCenter);
	} else {
		drawTextOnScreen(font, calculateProportionalSizeOrDistance(gameListXInCustom), position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignRight);
	}
}

void drawNonShadedGameNameOnScreen(char *buf, int position) {
	drawTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignCenter);

}

void drawNonShadedGameNameOnScreenPicMode(char *buf, int position) {
	int color[3] = {255,255,255};
	drawTextOnScreen(font, calculateProportionalSizeOrDistance(5), position, buf, color, VAlignMiddle | HAlignLeft);
}

void drawPictureTextOnScreen(char *buf) {
	if(!footerVisibleInFullscreenMode) {
		return;
	}
	int white[3]={255, 255, 255};
	drawTransparentRectangleToScreen(SCREEN_WIDTH, calculateProportionalSizeOrDistance((19*fontSize)/baseFont), 0, footerOnTop?0:SCREEN_HEIGHT-calculateProportionalSizeOrDistance((19*fontSize)/baseFont), (int[]){0,0,0}, 180);
	drawTextOnScreen(font, SCREEN_WIDTH/2, footerOnTop?calculateProportionalSizeOrDistance(((17*fontSize)/baseFont)+currentMode+(currentMode>0?0:0)):calculateProportionalSizeOrDistance(239+currentMode), buf, white, VAlignTop | HAlignCenter);
}

void drawImgFallbackTextOnScreen(char *fallBackText) {
	int white[3]={0, 0, 0};
	drawTextOnScreen(font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(120), fallBackText, white, VAlignMiddle | HAlignCenter);
}

void drawTextOnFooter(const char text[64]) {
	switch (currentMode) {
	case 0:
		drawTextOnScreen(footerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(footerPositionInSimple), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	case 1:
		drawTextOnScreen(footerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(footerPositionInTraditional), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	case 2:
		drawTextOnScreen(footerFont, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(footerPositionInDrunkenMonkey), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	}
}

void drawTextOnFooterWithColor(const char text[64], int txtColor[]) {
	drawTextOnScreen(footerFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), text, txtColor, VAlignMiddle| HAlignCenter);
}

void drawTextOnSettingsFooterWithColor(const char text[64], int txtColor[]) {
	drawTextOnScreen(settingsFooterFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(9), text, txtColor, VAlignMiddle| HAlignCenter);
}

void drawTextOnHeader(char *text) {
	switch (currentMode) {
	case 0:
		drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(headerPositionInSimple), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	case 1:
		drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(headerPositionInTraditional), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	case 2:
		drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(headerPositionInDrunkenMonkey), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
		break;
	case 3:
		if(text1YInCustom!=-1&&text1XInCustom!=-1) {
			drawCustomText1OnScreen(customHeaderFont, text1XInCustom, text1YInCustom, text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | text1AlignmentInCustom);
		}
		break;
	}
}

void drawTextOnHeaderWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(13), text, txtColor, VAlignMiddle | HAlignCenter);
}

void drawTextOnSettingsHeaderWithColor(char *text, int txtColor[]) {
	drawTextOnScreen(settingsHeaderFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(13), text, txtColor, VAlignMiddle | HAlignCenter);
}

void drawCurrentLetter(char *letter, int textColor[], int x, int y) {
	if (!fullscreenMode) {
		if (currentMode==0) {
			drawTextOnScreen(BIGFont, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
		} else {
			drawTextOnScreen(font, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
		}
	} else {
		drawTextOnScreen(font, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
	}
}

void drawShutDownText(const char text[64]) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(BIGFont, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text, white, VAlignMiddle | HAlignCenter);
}

void drawTimeOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(316), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignRight);
}

void drawBatteryOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(4), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignLeft);
}

void drawCurrentExecutable(char *executable, int textColor[]) {
	drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), executable, textColor, VAlignMiddle | HAlignCenter);
}

void drawCurrentSectionGroup(char *groupName, int textColor[]) {
	drawTextOnScreen(BIGFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2), groupName, textColor, VAlignMiddle | HAlignCenter);
}

void drawError(char *errorMessage, int textColor[]) {
	if(strchr(errorMessage,'-')==NULL) {
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), errorMessage, textColor, VAlignMiddle | HAlignCenter);
	} else {
		char *line2 = strchr(errorMessage,'-');
		int index = (line2-errorMessage);
		line2++;
		char line1[200];
		strcpy(line1, errorMessage);
		line1[index]='\0';
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)-calculateProportionalSizeOrDistance(12), line1, textColor, VAlignMiddle | HAlignCenter);
		drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3)+calculateProportionalSizeOrDistance(12), line2, textColor, VAlignMiddle | HAlignCenter);
	}
}

SDL_Rect drawRectangleToScreen(int width, int height, int x, int y, int rgbColor[]) {
	SDL_Rect rectangle;
	rectangle.w = width;
	rectangle.h = height;
	rectangle.x = x;
	rectangle.y = y;
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
	transparentrectangle = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 16, rgbColor[0], rgbColor[1], rgbColor[2], 0);
	SDL_SetAlpha(transparentrectangle, SDL_SRCALPHA, opacity); //128 is the amount of transparency or opacity (dont remember)
	SDL_Rect rectangleDest;
	rectangleDest.w = w;
	rectangleDest.h = h;
	rectangleDest.x = x;
	rectangleDest.y = y;
	//	SDL_FillRect(transparentrectangle, &rectangleDest, SDL_MapRGB(transparentrectangle->format, rgbColor[0], rgbColor[1], rgbColor[2]));
	SDL_BlitSurface(transparentrectangle, NULL, screen, &rectangleDest);
	SDL_FreeSurface(transparentrectangle);
}

int drawImage1(SDL_Surface* display, SDL_Surface *image, int x, int y, int xx, int yy , const double newwidth, const double newheight, int transparent, int smoothing) {
	// Zoom function uses doubles for rates of scaling, rather than
	// exact size values. This is how we get around that:
	double zoomx = newwidth  / (float)image->w;
	double zoomy = newheight / (float)image->h;
	// This function assumes no smoothing, so that any colorkeys wont bleed.
//	printf("%d,%d\n",(int)newwidth,(int)(image->w/2));
	SDL_Surface* sized = NULL;
	if (((int)newwidth<(int)(image->w/2))&&(int)(image->w/2)%(int)newwidth==0) {
//		printf("2\n");
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

int drawImage(pthread_t *myThread, SDL_Surface *display, SDL_Surface *image, int x, int y, int xx, int yy , const double newwidth, const double newheight, int transparent, int smoothing) {

	threadPicture picture;

	picture.display = display;
	picture.image=image;
	picture.x=x;
	picture.y=y;
	picture.xx=xx;
	picture.yy=yy;
	picture.newwidth=newwidth;
	picture.newheight=newheight;
	picture.transparent=transparent;
	picture.smoothing=smoothing;

	pthread_create(myThread, NULL, thread_func, &picture); // no parentheses here
	pthread_join(*myThread,NULL);
	return 1;
}

void showHeart(int x, int y) {
	SDL_Surface *img = IMG_Load(favoriteIndicator);
	SDL_Rect rectangleDest;
	rectangleDest.w = 0;
	rectangleDest.h = 0;
	rectangleDest.x = x;
	rectangleDest.y = y;
	SDL_BlitSurface(img, NULL, screen, &rectangleDest);
	SDL_FreeSurface(img);
}

void displayImageOnScreenTraditional(char *fileName) {
	SDL_Surface *screenshot = IMG_Load(fileName);
	int rightRectangleColor[3] = {80, 80, 80};
	int screenDivisions=(SCREEN_RATIO*5)/1.33;
	pthread_t myThread;

	drawTransparentRectangleToScreen((SCREEN_WIDTH/screenDivisions)*2,SCREEN_HEIGHT-calculateProportionalSizeOrDistance(43),SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)*2,calculateProportionalSizeOrDistance(22),rightRectangleColor,60);

	if (screenshot!=NULL) {
		double w = screenshot->w;
		double h = screenshot->h;
		double ratio = 0;  // Used for aspect ratio
		int smoothing = 1;
		ratio = w / h;   // get ratio for scaling image
		h = calculateProportionalSizeOrDistance(90);
		w = h*ratio;
		smoothing = 0;
		if (w!=2*(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(8)) {
			ratio = h / w;   // get ratio for scaling image
			w = 2*(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(8);
			h = w*ratio;
			int max = 90;
			int newMax = 90;
			if(!(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)) {
				max = 116;
				newMax = 100;
			}
			if (h>calculateProportionalSizeOrDistance(max)) {
				ratio = w / h;   // get ratio for scaling image
				h = calculateProportionalSizeOrDistance(newMax);
				w = h*ratio;
			}
			smoothing=1;
		}
		drawRectangleToScreen(w+calculateProportionalSizeOrDistance(4),	h+calculateProportionalSizeOrDistance(4), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(w/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance(24),CURRENT_SECTION.headerAndFooterBackgroundColor);
		drawTransparentRectangleToScreen(w,h,SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(w/2),calculateProportionalSizeOrDistance(26),rightRectangleColor,125);
		pthread_t myThread;
		drawImage(&myThread, screen, screenshot, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(w/2), calculateProportionalSizeOrDistance(26), 0, 0, w, h, 0, smoothing);
		if(hideHeartTimer!=NULL) {
			SDL_Surface *heart = IMG_Load(favoriteIndicator);
			if (heart!=NULL) {
				double wh = heart->w;
				double hh = heart->h;
				double ratioh = 0;  // Used for aspect ratio
				ratioh = wh / hh;   // get ratio for scaling image
				hh = calculateProportionalSizeOrDistance(heart->h);
				if(hh!=heart->h) {
					smoothing = 1;
				}
				wh = hh*ratioh;
				drawImage(&myThread, screen, heart, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(wh/2), calculateProportionalSizeOrDistance(26)+h/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
		//		pthread_join(myThread,NULL);
	} else {
		if(!(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)) {
			drawRectangleToScreen(calculateProportionalSizeOrDistance(138),calculateProportionalSizeOrDistance(104), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(calculateProportionalSizeOrDistance(134)/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance(24),CURRENT_SECTION.headerAndFooterBackgroundColor);
			drawTransparentRectangleToScreen(calculateProportionalSizeOrDistance(134),calculateProportionalSizeOrDistance(100),SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(134/2),calculateProportionalSizeOrDistance(26),rightRectangleColor,125);
		} else {
			drawRectangleToScreen(calculateProportionalSizeOrDistance(124),calculateProportionalSizeOrDistance(94), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(calculateProportionalSizeOrDistance(120)/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance(24),CURRENT_SECTION.headerAndFooterBackgroundColor);
			drawTransparentRectangleToScreen(calculateProportionalSizeOrDistance(120),calculateProportionalSizeOrDistance(90),SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(120/2),calculateProportionalSizeOrDistance(26),rightRectangleColor,125);
		}
		int smoothing = 0;
		if(hideHeartTimer!=NULL) {
			SDL_Surface *heart = IMG_Load(favoriteIndicator);
			if (heart!=NULL) {
				double wh = heart->w;
				double hh = heart->h;
				double ratioh = 0;  // Used for aspect ratio
				ratioh = wh / hh;   // get ratio for scaling image
				hh = calculateProportionalSizeOrDistance(heart->h);
				if(hh!=heart->h) {
					smoothing = 1;
				}
				wh = hh*ratioh;
				drawImage(&myThread, screen, heart, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(wh/2), calculateProportionalSizeOrDistance(26)+calculateProportionalSizeOrDistance(90)/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
		//		pthread_join(myThread,NULL);
	}

	SDL_Surface *systemImage = IMG_Load(CURRENT_SECTION.systemPicture);
	if (systemImage!=NULL) {
		double w1 = systemImage->w;
		double h1 = systemImage->h;
		double ratio1 = 0;  // Used for aspect ratio
		ratio1 = w1 / h1;   // get ratio for scaling image
		h1 = (SCREEN_HEIGHT*90)/240;
		w1 = h1*ratio1;
		ratio1 = h1 / w1;   // get ratio for scaling image
		int middleBottom = calculateProportionalSizeOrDistance(168)-h1/2;
		int smoothing=0;
		if(!(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)) {
			middleBottom = calculateProportionalSizeOrDistance(174)-h1/2;
			smoothing=1;
		}
		pthread_t myThread;
		drawImage(&myThread, screen, systemImage, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions)-(w1/2), middleBottom, 0, 0, w1, h1, 0, smoothing);
	}
}

void displayImageOnScreenCustom(char *fileName) {
//	printf("%s\n",fileName);
	SDL_Surface *screenshot = IMG_Load(fileName);

	if (screenshot!=NULL) {
		double w = screenshot->w;
		double h = screenshot->h;
		double ratio = 0;  // Used for aspect ratio
		int smoothing = 1;
		ratio = w / h;   // get ratio for scaling image
		h = calculateProportionalSizeOrDistance(artHeightInCustom);
		w = h*ratio;
		smoothing = 0;
		if (w!=calculateProportionalSizeOrDistance(artWidthInCustom)) {
			ratio = h / w;   // get ratio for scaling image
			w = calculateProportionalSizeOrDistance(artWidthInCustom);
			h = w*ratio;
			smoothing=1;
		}
		smoothing=0;
		pthread_t myThread;
		drawImage(&myThread,screen, screenshot, calculateProportionalSizeOrDistance(artXInCustom), calculateProportionalSizeOrDistance(artYInCustom), 0, 0, w, h, 0, smoothing);
		//	h	pthread_join(myThread,NULL);
		if(hideHeartTimer!=NULL) {
			SDL_Surface *heart = IMG_Load(favoriteIndicator);
			if (heart!=NULL) {
				double wh = heart->w;
				double hh = heart->h;
				double ratioh = 0;  // Used for aspect ratio
				ratioh = wh / hh;   // get ratio for scaling image
				hh = calculateProportionalSizeOrDistance(heart->h);
				if(hh!=heart->h) {
					smoothing = 1;
					wh = hh*ratioh;
				}
				drawImage(&myThread, screen, heart, calculateProportionalSizeOrDistance(artXInCustom)+w/2-(wh/2), calculateProportionalSizeOrDistance(artYInCustom)+h/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
		if(artTextDistanceFromPictureInCustom>0) {
			char temp[500];
			snprintf(temp,sizeof(temp),"%d/%d", CURRENT_SECTION.realCurrentGameNumber, CURRENT_SECTION.gameCount);
			drawCustomGameNameUnderPictureOnScreen(currentGameNameBeingDisplayed, calculateProportionalSizeOrDistance(artXInCustom)+w/2, calculateProportionalSizeOrDistance(artYInCustom)+h+calculateProportionalSizeOrDistance(artTextDistanceFromPictureInCustom),calculateProportionalSizeOrDistance(artWidthInCustom));
		}
	} else {
		int smoothing = 0;
		if(hideHeartTimer!=NULL) {
			SDL_Surface *heart = IMG_Load(favoriteIndicator);
			if (heart!=NULL) {
				double wh = heart->w;
				double hh = heart->h;
				double ratioh = 0;  // Used for aspect ratio
				ratioh = wh / hh;   // get ratio for scaling image
				hh = calculateProportionalSizeOrDistance(heart->h);
				if(hh!=heart->h) {
					smoothing = 1;
				}
				wh = hh*ratioh;
				pthread_t myThread;
				drawImage(&myThread, screen, heart, calculateProportionalSizeOrDistance(artXInCustom)+artWidthInCustom/2-(wh/2), calculateProportionalSizeOrDistance(artYInCustom)+artHeightInCustom/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
	}
	//	pthread_join(myThread,NULL);
	if(systemXInCustom>0&&systemYInCustom>0) {
		SDL_Surface *systemImage = IMG_Load(CURRENT_SECTION.systemPicture);
		if (systemImage!=NULL) {
			double w1 = systemImage->w;
			double h1 = systemImage->h;
			double ratio1 = 0;  // Used for aspect ratio
			ratio1 = w1 / h1;   // get ratio for scaling image
			h1 = calculateProportionalSizeOrDistance(systemHeightInCustom);
			w1 = h1*ratio1;
			int smoothing=1;
			pthread_t myThread;
			drawImage(&myThread,screen, systemImage, calculateProportionalSizeOrDistance(systemXInCustom), calculateProportionalSizeOrDistance(systemYInCustom), 0, 0, w1, h1, 0, smoothing);
		}
	}
}

void displayHeart() {
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
			pthread_t myThread;
			drawImage(&myThread, screen, heart, SCREEN_WIDTH/2-(wh/2), SCREEN_HEIGHT/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			//			pthread_join(myThread,NULL);
		}
	}
}

void displayImageOnScreenDrunkenMonkey(char *fileName) {
	SDL_Surface *screenshot = IMG_Load(fileName);
	int rightRectangleColor[3] = {80, 80, 80};
	int screenDivisions=(SCREEN_RATIO*3)/1.33;
	pthread_t myThread;

	drawTransparentRectangleToScreen((SCREEN_WIDTH/screenDivisions),SCREEN_HEIGHT-calculateProportionalSizeOrDistance((22*fontSize)/baseFont)*2,SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions),calculateProportionalSizeOrDistance((22*fontSize)/baseFont),rightRectangleColor,60);

	if (screenshot!=NULL) {
		double w = screenshot->w;
		double h = screenshot->h;
		double ratio = 0;  // Used for aspect ratio
		int smoothing = 1;
		ratio = w / h;   // get ratio for scaling image
		h = calculateProportionalSizeOrDistance(72);
		w = h*ratio;
		smoothing = 0;
		if (w!=(SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(8)) {
			ratio = h / w;   // get ratio for scaling image
			w = (SCREEN_WIDTH/screenDivisions)-calculateProportionalSizeOrDistance(8);
			h = w*ratio;
			smoothing=1;
		}
		drawRectangleToScreen(w+calculateProportionalSizeOrDistance(4),	h+calculateProportionalSizeOrDistance(4), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(w/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance((24*fontSize)/baseFont),CURRENT_SECTION.headerAndFooterBackgroundColor);
		drawTransparentRectangleToScreen(w,h,SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(w/2),calculateProportionalSizeOrDistance((27*fontSize)/baseFont),rightRectangleColor,125);
		drawImage(&myThread, screen, screenshot, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(w/2), calculateProportionalSizeOrDistance((27*fontSize)/baseFont), 0, 0, w, h, 0, smoothing);
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
				drawImage(&myThread, screen, heart, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(wh/2), calculateProportionalSizeOrDistance((27*fontSize)/baseFont)+h/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
	} else {

		if(!(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)) {
			drawRectangleToScreen(calculateProportionalSizeOrDistance(102),calculateProportionalSizeOrDistance(78), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(calculateProportionalSizeOrDistance(98)/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance((24*fontSize)/baseFont),CURRENT_SECTION.headerAndFooterBackgroundColor);
			drawTransparentRectangleToScreen(calculateProportionalSizeOrDistance(98),calculateProportionalSizeOrDistance(74),SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-calculateProportionalSizeOrDistance(98/2),calculateProportionalSizeOrDistance((27*fontSize)/baseFont),rightRectangleColor,125);
		} else {
			drawRectangleToScreen(calculateProportionalSizeOrDistance(96)+calculateProportionalSizeOrDistance(4), calculateProportionalSizeOrDistance(72)+calculateProportionalSizeOrDistance(4), SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(calculateProportionalSizeOrDistance(96)/2)-calculateProportionalSizeOrDistance(2), calculateProportionalSizeOrDistance((24*fontSize)/baseFont),CURRENT_SECTION.headerAndFooterBackgroundColor);
			drawTransparentRectangleToScreen(calculateProportionalSizeOrDistance(96),calculateProportionalSizeOrDistance(72),SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(calculateProportionalSizeOrDistance(96)/2),calculateProportionalSizeOrDistance((27*fontSize)/baseFont),rightRectangleColor,125);
		}
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
				drawImage(&myThread, screen, heart, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(wh/2), calculateProportionalSizeOrDistance((27*fontSize)/baseFont)+calculateProportionalSizeOrDistance(72)/2-hh/2, 0, 0, wh, hh, 0, smoothing);
			}
		}
	}

	SDL_Surface *systemImage = IMG_Load(CURRENT_SECTION.systemPicture);
	if (systemImage!=NULL) {
		double w1 = systemImage->w;
		double h1 = systemImage->h;
		double ratio1 = 0;  // Used for aspect ratio
		ratio1 = w1 / h1;   // get ratio for scaling image
		h1 = (SCREEN_HEIGHT*72)/240;
		w1 = h1*ratio1;
		ratio1 = h1 / w1;   // get ratio for scaling image
		int middleBottom = SCREEN_HEIGHT-h1-calculateProportionalSizeOrDistance(18);
		int smoothing=1;
		if(!(SCREEN_RATIO>=1.33&&SCREEN_RATIO<=1.34)) {
			middleBottom = SCREEN_HEIGHT-calculateProportionalSizeOrDistance(16)-h1;
			smoothing=1;
		}
		pthread_t myThread;
		drawImage(&myThread,screen, systemImage, SCREEN_WIDTH-(SCREEN_WIDTH/screenDivisions/2)-(w1/2), middleBottom, 0, 0, w1, h1, 0, smoothing);
	}
	//	pthread_join(myThread,NULL);
}

void* thread_func(void *picture) {
	// Zoom function uses doubles for rates of scaling, rather than
	// exact size values. This is how we get around that:
	double zoomx = ((threadPicture*)picture)->newwidth  / (float)((threadPicture*)picture)->image->w;
	double zoomy = ((threadPicture*)picture)->newheight / (float)((threadPicture*)picture)->image->h;
	// This function assumes no smoothing, so that any colorkeys wont bleed.
//	printf("%d,%d\n",(int)((threadPicture*)picture)->newwidth,(int)(image->w/2));
	SDL_Surface* sized = NULL;
	if (((int)((threadPicture*)picture)->newwidth<(int)(((threadPicture*)picture)->image->w/2))&&((int)(((threadPicture*)picture)->image->w/2)%(int)((threadPicture*)picture)->newwidth)==0) {
//		printf("2\n");
		zoomx = (float)((threadPicture*)picture)->image->w/((threadPicture*)picture)->newwidth;
		zoomy = (float)((threadPicture*)picture)->image->h/((threadPicture*)picture)->newheight;
		sized = shrinkSurface(((threadPicture*)picture)->image, zoomx, zoomy);
	} else {
		zoomx = ((threadPicture*)picture)->newwidth  / (float)((threadPicture*)picture)->image->w;
		zoomy = ((threadPicture*)picture)->newheight / (float)((threadPicture*)picture)->image->h;
		sized = zoomSurface(((threadPicture*)picture)->image, zoomx, zoomy, ((threadPicture*)picture)->smoothing);
	}	// If the original had an alpha color key, give it to the new one.
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

SDL_Surface *resizeSurface (SDL_Surface *surface) {
	double w = surface->w;
	double h = surface->h;
	double ratio = 0;  // Used for aspect ratio
	int smoothing = 0;
	ratio = w / h;   // get ratio for scaling image
	h = SCREEN_HEIGHT;
	w = h*ratio;
	if (w!=SCREEN_WIDTH) {
		ratio = h / w;   // get ratio for scaling image
		w = SCREEN_WIDTH;
		h = w*ratio;
		smoothing=1;
	}
	double zoomx = SCREEN_WIDTH / surface->w;
	double zoomy = SCREEN_HEIGHT / surface->h;

	SDL_Surface *sized = NULL;
	sized = zoomSurface(surface, zoomx, zoomy, smoothing);

	if(surface->flags & SDL_SRCCOLORKEY ) {
		Uint32 colorkey = surface->format->colorkey;
		SDL_SetColorKey(sized, SDL_SRCCOLORKEY, colorkey);
	}
	free(surface);

	return sized;
}

void resizeSectionBackground(struct MenuSection *section) {
	section->background = resizeSurface(section->background);
}

void resizeGroupBackground(struct SectionGroup *group) {
//	printf("before %d\n",group->groupBackgroundSurface->w);
	group->groupBackgroundSurface = resizeSurface(group->groupBackgroundSurface);
//	printf("after %d\n",group->groupBackgroundSurface->w);
}

void displayCenteredSurface(SDL_Surface *surface) {
	SDL_Rect rectangleDest;
	rectangleDest.w = 0;
	rectangleDest.h = 0;
	rectangleDest.x = SCREEN_WIDTH/2-surface->w/2;
	rectangleDest.y = ((SCREEN_HEIGHT)/2-surface->h/2);
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
				pthread_t myThread;
				drawImage(&myThread, screen, img, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2-h/2, 0, 0, w, h, 0, smoothing);
			}
			//			pthread_join(myThread,NULL);
		}
	}
}

void drawUSBScreen() {
	int white[3]={255, 255, 255};
	int black[3]={0, 0, 0};
	displayCenteredImageOnScreen("./usb.png"," ",1,0);
	drawTextOnScreen(headerFont,163,27,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,29,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,27,"USB MODE",white,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,217,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,219,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,217,"PRESS START TO END",white,VAlignMiddle | HAlignCenter);
}

void initializeDisplay() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
	SCREEN_WIDTH = info->current_w;
	SCREEN_HEIGHT = info->current_h;
	#ifdef TARGET_PC
	SCREEN_WIDTH = 320;
	SCREEN_HEIGHT = 240;
	#endif
	if (SCREEN_WIDTH<320||SCREEN_HEIGHT<240) {
		SCREEN_WIDTH = 320;
		SCREEN_HEIGHT = 240;
	}

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	FILE *fpHDMI = fopen("/sys/class/hdmi/hdmi","r");
	if (fpHDMI!=NULL) {
		read = getline(&line, &len, fpHDMI);
		hdmiEnabled = atoi(line);
		fclose(fpHDMI);
	}
	if (line) {
		free(line);
	}

	hdmiChanged = hdmiEnabled;
	if (hdmiEnabled) {
		SCREEN_WIDTH = 640;
		SCREEN_HEIGHT = 480;
	}
	SCREEN_RATIO = (double)SCREEN_WIDTH/SCREEN_HEIGHT;
	SDL_ShowCursor(0);
	//	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
	TTF_Init();
	MAGIC_NUMBER = SCREEN_WIDTH-calculateProportionalSizeOrDistance(2);
}



void refreshScreen() {
	SDL_Flip(screen);
}

void initializeSettingsFonts() {
	settingsfont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(14));
	settingsHeaderFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(20));
	settingsFooterFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(16));
}

void initializeFonts() {
	TTF_Init();
	font = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize));
	miniFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(artTextFontSizeInCustom));
	picModeFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+5));
	BIGFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+18));
	headerFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+6));
	footerFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(fontSize+2));
	customHeaderFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(text1FontSizeInCustom));
	//	customCurrentGameFont = TTF_OpenFont(menuFont, calculateProportionalSizeOrDistance(text3FontSizeInCustom));
}

void freeFonts() {
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(customHeaderFont);
	customHeaderFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	TTF_CloseFont(picModeFont);
	picModeFont = NULL;
	TTF_CloseFont(miniFont);
	miniFont = NULL;
	TTF_CloseFont(BIGFont);
	footerFont = NULL;
}

void freeSettingsFonts() {
	TTF_CloseFont(settingsfont);
	settingsfont = NULL;
	TTF_CloseFont(settingsHeaderFont);
	settingsHeaderFont = NULL;
	TTF_CloseFont(settingsFooterFont);
	settingsFooterFont = NULL;
}

void freeResources() {
	//    pthread_join(clockThread, NULL);
	//    pthread_mutex_destroy(&lock);
	freeFonts();
	freeSettingsFonts();
	TTF_Quit();
	#ifdef TARGET_RG350
	Shake_Stop(device, effect_id);
	Shake_EraseEffect(device, effect_id);
	Shake_Close(device);
	Shake_Quit();
	#endif
	SDL_Quit();
}
