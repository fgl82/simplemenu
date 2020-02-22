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

SDL_Surface *screen = NULL;
TTF_Font *font = NULL;
TTF_Font *miniFont = NULL;
TTF_Font *picModeFont = NULL;
TTF_Font *BIGFont = NULL;
TTF_Font *headerFont = NULL;
TTF_Font *footerFont = NULL;

SDL_Color make_color(Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color c= { r, g, b };
	return c;
}

int calculateProportionalSizeOrDistance(int number) {
	return (SCREEN_HEIGHT*number)/240;
	//	return (number*SCREEN_WIDTH)/SCREEN_HEIGHT;
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
	int width = calculateProportionalSizeOrDistance(315);
	if (fullscreenMode) {
		width = calculateProportionalSizeOrDistance(315);
	}
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

int drawShadedTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align, int backgroundColor[]) {
	return genericDrawTextOnScreen(font, x, y, buf, txtColor, align, backgroundColor, 1);
}

int drawTextOnScreen(TTF_Font *font, int x, int y, const char buf[300], int txtColor[], int align) {
	return genericDrawTextOnScreen(font, x, y, buf, txtColor, align, NULL, 0);
}

void drawShadedGameNameOnScreen(char *buf, int position) {
	drawShadedTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodySelectedTextTextColor, VAlignBottom | HAlignCenter, menuSections[currentSectionNumber].bodySelectedTextBackgroundColor);
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

void drawNonShadedGameNameOnScreen(char *buf, int position) {
	drawTextOnScreen(font, SCREEN_WIDTH/2, position, buf, menuSections[currentSectionNumber].bodyTextColor, VAlignBottom | HAlignCenter);
}

void drawNonShadedGameNameOnScreenPicMode(char *buf, int position) {
	int color[3] = {255,255,255};
	drawTextOnScreen(font, calculateProportionalSizeOrDistance(5), position, buf, color, VAlignMiddle | HAlignLeft);
}

void drawPictureTextOnScreen(char *buf) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(font, SCREEN_WIDTH/2, calculateProportionalSizeOrDistance(239), buf, white, VAlignTop | HAlignCenter);
//	drawTextOnScreen(font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(239), buf, white, VAlignTop | HAlignCenter);
}

void drawImgFallbackTextOnScreen(char *fallBackText) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(font, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(120), fallBackText, white, VAlignTop | HAlignCenter);
}

void drawTextOnFooter(const char text[64]) {
	drawTextOnScreen(footerFont, SCREEN_WIDTH/2, SCREEN_HEIGHT-calculateProportionalSizeOrDistance(8), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignCenter);
}

void drawShutDownText(const char text[64]) {
	int white[3]={255, 255, 255};
	drawTextOnScreen(BIGFont, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, text, white, VAlignMiddle | HAlignCenter);
}

void drawTextOnHeader(char *text) {
	drawTextOnScreen(headerFont, (SCREEN_WIDTH/2), calculateProportionalSizeOrDistance(24), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignTop | HAlignCenter);
}

void drawTimeOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(316), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignRight);
}

void drawBatteryOnFooter(char *text) {
	drawTextOnScreen(font,calculateProportionalSizeOrDistance(4), calculateProportionalSizeOrDistance(232), text, menuSections[currentSectionNumber].headerAndFooterTextColor, VAlignMiddle | HAlignLeft);
}

void drawCurrentLetter(char *letter, int textColor[], int x, int y) {
	if (!fullscreenMode) {
		drawTextOnScreen(BIGFont, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
	} else {
		drawTextOnScreen(miniFont, x, y, letter, textColor, VAlignMiddle | HAlignCenter);
	}
}

void drawCurrentExecutable(char *executable, int textColor[]) {
	drawTextOnScreen(footerFont, (SCREEN_WIDTH/2), (SCREEN_HEIGHT/2)+calculateProportionalSizeOrDistance(3), executable, textColor, VAlignMiddle | HAlignCenter);
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

SDL_Rect drawRectangleOnScreen(int width, int height, int x, int y, int rgbColor[]) {
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
	SDL_Rect bgrect = drawRectangleOnScreen(img->w, img->h, (SCREEN_WIDTH/2)-(img->w/2),(SCREEN_HEIGHT/2)-(img->h/2), rgbColor);
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

int drawImage(SDL_Surface* display, SDL_Surface *image, const char * filename, int x, int y, int xx, int yy , const double newwidth, const double newheight, int transparent, int smoothing) {
	// Zoom function uses doubles for rates of scaling, rather than
	// exact size values. This is how we get around that:
	double zoomx = newwidth  / (float)image->w;
	double zoomy = newheight / (float)image->h;
	// This function assumes no smoothing, so that any colorkeys wont bleed.
	SDL_Surface* sized = zoomSurface( image, zoomx, zoomy, smoothing);
	// If the original had an alpha color key, give it to the new one.
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

void displayImageOnScreen(char *fileName, char *fallBackText) {
	SDL_Surface *img = IMG_Load(fileName);
	if (img==NULL) {
		drawImgFallbackTextOnScreen(fallBackText);
	} else {
		if (img->h==SCREEN_HEIGHT || img->w==SCREEN_WIDTH) {
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
			if (strstr(fileName,CURRENT_SECTION.consolePicture)) {
				w=calculateProportionalSizeOrDistance(img->w);
				h=w*ratio;
			}
			if ((int)h!=(int)img->h) {
				smoothing = 1;
			}
			drawImage(screen, img, fileName, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT/2-h/2, 0, 0, w, h, 0, smoothing);
		}
	}
}

void drawUSBScreen() {
	int white[3]={255, 255, 255};
	int black[3]={0, 0, 0};
	displayImageOnScreen("./resources/usb.png","");
	drawTextOnScreen(headerFont,163,29,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,215,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,31,"USB MODE",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,163,217,"PRESS START TO END",black,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,29,"USB MODE",white,VAlignMiddle | HAlignCenter);
	drawTextOnScreen(headerFont,161,215,"PRESS START TO END",white,VAlignMiddle | HAlignCenter);
}

void initializeDisplay() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();   //<-- calls SDL_GetVideoInfo();
	SCREEN_HEIGHT = info->current_h;
	if (SCREEN_HEIGHT>768) {
		SCREEN_HEIGHT = 768;
	}
	if (SCREEN_HEIGHT<240) {
		SCREEN_HEIGHT = 240;
	}
	SDL_ShowCursor(0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE | SDL_NOFRAME);
}

void refreshScreen() {
	SDL_Flip(screen);
}

void initializeFonts() {
	TTF_Init();
	font = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(14));
	miniFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(14));
	picModeFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(19));
	BIGFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(32));
	headerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(20));
	footerFont = TTF_OpenFont("resources/akashi.ttf", calculateProportionalSizeOrDistance(16));
}

void freeResources() {
	//    pthread_join(clockThread, NULL);
	//    pthread_mutex_destroy(&lock);
	TTF_CloseFont(font);
	font = NULL;
	TTF_CloseFont(headerFont);
	headerFont = NULL;
	TTF_CloseFont(footerFont);
	footerFont = NULL;
	TTF_Quit();
	#ifdef TARGET_RG350
	Shake_Stop(device, effect_id);
	Shake_EraseEffect(device, effect_id);
	Shake_Close(device);
	Shake_Quit();
	#endif
	SDL_Quit();
}
