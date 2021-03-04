#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/logic.h"
#include "../headers/string_utils.h"
#include "../headers/ini2.h"
#include "../headers/graphics.h"
#include "../headers/utils.h"

char home[5000];
char pathToThemeConfigFile[1000];
char pathToThemeConfigFilePlusFileName[1000];

int atoifgl(const char* value) {
	if (value!=NULL) {
		return atoi(value);
	} else {
		return 0;
	}

}

void loadAliasList(int sectionNumber) {
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE *aliasFile=getCurrentSectionAliasFile();
	if (aliasFile==NULL) {
		strcpy(menuSections[sectionNumber].aliasFileName," ");
		return;
	}
	menuSections[sectionNumber].aliasHashTable = ht_create(MAX_GAMES_IN_SECTION);
	while ((read = getline(&line, &len, aliasFile)) != -1) {
		char *romName = strtok(line, "=");
		char *alias = strtok(NULL, "=");
		int lastChar = strlen(alias)-1;
		alias[lastChar]='\0';
		ht_set(menuSections[sectionNumber].aliasHashTable, romName, alias);
	}
	if (line) {
		free(line);
	}
	fclose(aliasFile);
}

void checkIfDefault() {
	FILE *fp=NULL;
	FILE *fpScripts=NULL;
	#ifdef TARGET_BITTBOY
	fp = fopen("/mnt/autoexec.sh", "r");
	fpScripts = fopen("scripts/autoexec.sh", "r");
	#endif
	#ifdef TARGET_RG300
	fp = fopen("/home/retrofw/autoexec.sh", "r");
	fpScripts = fopen("scripts/autoexec.sh", "r");
	#endif
	#if defined TARGET_RG350
	fp = fopen("/media/data/local/sbin/frontend_start", "r");
	fpScripts = fopen("scripts/frontend_start", "r");
	#endif
	#if defined TARGET_RG350_BETA
	fp = fopen("/media/data/local/home/.autostart", "r");
	fpScripts = fopen("scripts/frontend_start", "r");
	#endif
	#ifdef TARGET_NPG
	fp = fopen("/media/data/local/sbin/frontend_start", "r");
	fpScripts = fopen("scripts/frontend_start", "r");
	#endif
	shutDownEnabled=1;
	int sameFile=1;
	int c1, c2;
	if (fp==NULL) {
		shutDownEnabled=0;
		return;
	}
	c1 = getc(fp);
	c2 = getc(fpScripts);
	while (sameFile && c1 != EOF && c2 != EOF) {
		if (c1 != c2) {
			sameFile=0;
			shutDownEnabled=0;
			break;
		}
		c1 = getc(fp);
		c2 = getc(fpScripts);
	}
	if (fp!=NULL) {
		fclose(fp);
	}
	if (fpScripts!=NULL) {
		fclose(fpScripts);
	}
}

uint32_t hex2int(char *hex) {
	char *hexCopy = malloc(strlen(hex)+1);
	strcpy(hexCopy,hex);
	uint32_t val = 0;
	int i=0;
	while (*hexCopy) {
		i++;
		// get current character then increment
		uint8_t byte = *hexCopy++;
		// transform hex character to the 4bit equivalent number, using the ascii table indexes
		if (byte >= '0' && byte <= '9') byte = byte - '0';
		else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
		else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
		// shift 4 to make space for new digit, and add the 4 bits of the new digit
		val = (val << 4) | (byte & 0xF);
	}
	hexCopy-=i;
	free(hexCopy);
	return val;
}

void setRGBFromHex (int *rgbColor, const char *hexColor) {
	char r[3];
	char g[3];
	char b[3];
	r[0]=hexColor[0];
	r[1]=hexColor[1];
	r[2]='\0';
	g[0]=hexColor[2];
	g[1]=hexColor[3];
	g[2]='\0';
	b[0]=hexColor[4];
	b[1]=hexColor[5];
	b[2]='\0';
	rgbColor[0]=hex2int(r);
	rgbColor[1]=hex2int(g);
	rgbColor[2]=hex2int(b);
}


void setStringValueInSection(ini_t *config, char *sectionName, char *valueName, char *sectionValueToBeSet,char *fallback) {
	const char *value;
	value = ini_get(config, sectionName, valueName);
	if(value==NULL) {
		strcpy(sectionValueToBeSet,fallback);
		return;
	}
	strcpy(sectionValueToBeSet,value);
}

void setRGBColorInSection(ini_t *config, char *sectionName, char *valueName, int *sectionValueToBeSet) {
	const char *value;
	value = ini_get(config, sectionName, valueName);
	if (value==NULL) {
		value = ini_get(config, "DEFAULT", valueName);
	}
	setRGBFromHex(sectionValueToBeSet, value);
}

void setThemeResourceValueInSection(ini_t *config, char *sectionName, char *valueName, char *sectionValueToBeSet) {
	const char *value;
	value = ini_get(config, sectionName, valueName);
	if(value==NULL) {
		value = ini_get(config, "DEFAULT", valueName);
		if (value==NULL) {
			value = ini_get(config, "GENERAL", valueName);
		}
		if (value==NULL) {
			strcpy(sectionValueToBeSet, "");
			return;
		}
		strcpy(sectionValueToBeSet,pathToThemeConfigFile);
		strcat(sectionValueToBeSet,value);
		return;
	}
	strcpy(sectionValueToBeSet,pathToThemeConfigFile);
	strcat(sectionValueToBeSet,value);
}


void loadTheme(char *theme) {
	strcpy(pathToThemeConfigFilePlusFileName,theme);
	char *romPath = getRomPath(theme);
	char *temp = malloc(3000);
	strcpy(temp,romPath);
	strcat(temp,"/");
	strcpy(pathToThemeConfigFile,temp);
	free(temp);
	free(romPath);
	ini_t *themeConfig = ini_load(theme);
	const char *value;
	for (int i=0;i<menuSectionCounter;i++) {
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "headerBackGround", menuSections[i].headerAndFooterBackgroundColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "headerFont", menuSections[i].headerAndFooterTextColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "bodyBackground", menuSections[i].bodyBackgroundColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "bodyFont", menuSections[i].bodyTextColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "selectedItemBackground", menuSections[i].bodySelectedTextBackgroundColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "selectedItemFont", menuSections[i].bodySelectedTextTextColor);
		setRGBColorInSection(themeConfig, menuSections[i].sectionName, "pictureTextFont", menuSections[i].pictureTextColor);

		setThemeResourceValueInSection (themeConfig, menuSections[i].sectionName, "logo", menuSections[i].systemLogo);
		setThemeResourceValueInSection (themeConfig, menuSections[i].sectionName, "background", menuSections[i].background);

		value = ini_get(themeConfig, "GENERAL", "system_w_in_custom");
		systemWidth = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "system_h_in_custom");
		systemHeight = atoifgl(value);

		if (menuSections[i].systemLogoSurface!=NULL) {
			logMessage("INFO","loadTheme - Freeing system logo");
			SDL_FreeSurface(menuSections[i].systemLogoSurface);
			menuSections[i].systemLogoSurface = NULL;
		}
		if (menuSections[i].systemPictureSurface!=NULL) {
			logMessage("INFO","loadTheme - Freeing system picture");
			SDL_FreeSurface(menuSections[i].systemPictureSurface);
			menuSections[i].systemPictureSurface = NULL;
		}
		if (menuSections[i].backgroundSurface!=NULL) {
			logMessage("INFO","loadTheme - Freeing system background");
			SDL_FreeSurface(menuSections[i].backgroundSurface);
			menuSections[i].backgroundSurface = NULL;
		}

		if(i==currentSectionNumber) {
			logMessage("INFO","loadTheme - Loading system logo");
			menuSections[i].systemLogoSurface = IMG_Load(menuSections[i].systemLogo);
			resizeSectionSystemLogo(&menuSections[i]);
			logMessage("INFO","Loading system background");
			menuSections[i].backgroundSurface = IMG_Load(menuSections[i].background);
			resizeSectionBackground(&menuSections[i]);
			logMessage("INFO","Loading system picture");
			menuSections[i].systemPictureSurface = IMG_Load(menuSections[i].systemPicture);
			resizeSectionSystemPicture(&menuSections[i]);
		}

		setThemeResourceValueInSection (themeConfig, menuSections[i].sectionName, "system", menuSections[i].systemPicture);

		setThemeResourceValueInSection (themeConfig, "GENERAL", "favorite_indicator", favoriteIndicator);
		setThemeResourceValueInSection (themeConfig, "GENERAL", "font", menuFont);
		setThemeResourceValueInSection (themeConfig, "GENERAL", "section_groups_folder", sectionGroupsFolder);

		value = ini_get(themeConfig, "GENERAL", "colorful_fullscreen_menu");
		if (value == NULL) {
			colorfulFullscreenMenu = 0;
		} else {
			colorfulFullscreenMenu = atoifgl(value);
		}

		value = ini_get(themeConfig, "GENERAL", "font_outline");
		if (value == NULL) {
			fontOutline = 0;
		} else {
			fontOutline = atoifgl(value);
		}

		value = ini_get(themeConfig, "GENERAL", "display_section_group_name");
		if (value == NULL) {
			displaySectionGroupName = 0;
		} else {
			displaySectionGroupName = atoifgl(value);
		}

		value = ini_get(themeConfig, "GENERAL", "items_in_custom");
		itemsPerPage = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "items_separation_in_custom");
		itemsSeparation = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "items_in_full_custom");
		itemsPerPageFullscreen = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "game_list_alignment_in_custom");
		gameListAlignment = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "game_list_x_in_custom");
		gameListX = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "game_list_y_in_custom");
		gameListY = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "game_list_w_in_custom");
		gameListWidth = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "game_list_position_in_full_custom");
		gameListPositionFullScreen = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_max_w_in_custom");
		artWidth = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_max_h_in_custom");
		artHeight = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_x_in_custom");
		artX = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_y_in_custom");
		artY = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "system_x_in_custom");
		systemX = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "system_y_in_custom");
		systemY = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "font_size_custom");
		fontSize = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "text1_font_size_in_custom");
		text1FontSize = atoi (value);

		value = ini_get(themeConfig, "GENERAL", "text1_x_in_custom");
		text1X = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "text1_y_in_custom");
		text1Y = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "text1_alignment_in_custom");
		text1Alignment = atoifgl(value);

		setThemeResourceValueInSection (themeConfig, "GENERAL", "textX_font_custom", textXFont);

		value = ini_get(themeConfig, "GENERAL", "text2_font_size_in_custom");
		text2FontSize = atoi (value);

		value = ini_get(themeConfig, "GENERAL", "text2_x_in_custom");
		text2X = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "text2_y_in_custom");
		text2Y = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "text2_alignment_in_custom");
		text2Alignment = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_text_distance_from_picture_in_custom");
		artTextDistanceFromPicture = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_text_line_separation_in_custom");
		artTextLineSeparation = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "art_text_font_size_in_custom");
		artTextFontSize = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "font_size");
		baseFont = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "transparent_shading");
		transparentShading  = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "fullscreen_footer_on_top");
		footerOnTop = atoifgl(value);

    	currentMode=3;
    	MENU_ITEMS_PER_PAGE=itemsPerPage;
    	FULLSCREEN_ITEMS_PER_PAGE=itemsPerPageFullscreen;

		freeFonts();
		freeSettingsFonts();
		initializeSettingsFonts();
		initializeFonts();
	}
	ini_free(themeConfig);
}

void checkThemes() {
	char *files[1000];
	char tempString[1000];
	snprintf(tempString,sizeof(tempString),"%s/.simplemenu/themes/",getenv("HOME"));
	int n = findDirectoriesInDirectory(tempString, files, 0);
	qsort(files, n, sizeof(const char*), sortStringArray);
	for(int i=0;i<n;i++) {
		themes[i]=malloc(strlen(files[i])+1);
		strcpy(themes[i],files[i]);
		free(files[i]);
		themeCounter++;
	}
}

void createConfigFilesInHomeIfTheyDontExist() {
	snprintf(home,sizeof(home),"%s",getenv("HOME"));
	char pathToConfigFiles[5000];
	char pathToAppFiles[5000];
	char pathToGameFiles[5000];
	char pathToThemeFiles[5000];
	char pathToTempFiles[5000];
	char pathToSectionGroupsFiles[5000];
	snprintf(pathToConfigFiles,sizeof(pathToConfigFiles),"%s/.simplemenu",home);
	snprintf(pathToAppFiles,sizeof(pathToConfigFiles),"%s/.simplemenu/apps",home);
	snprintf(pathToGameFiles,sizeof(pathToGameFiles),"%s/.simplemenu/games",home);
	snprintf(pathToSectionGroupsFiles,sizeof(pathToSectionGroupsFiles),"%s/.simplemenu/section_groups",home);
	snprintf(pathToThemeFiles,sizeof(pathToThemeFiles),"%s/.simplemenu/themes",home);
	snprintf(pathToTempFiles,sizeof(pathToTempFiles),"%s/.simplemenu/tmp",home);
	int directoryExists=mkdir(pathToConfigFiles,0700);
	if (!directoryExists) {
		char copyCommand[5000];
		snprintf(copyCommand,sizeof(copyCommand),"cp ./config/* %s/.simplemenu/",home);
		int ret = system(copyCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyAppsCommand[5000];
		mkdir(pathToAppFiles,0700);
		snprintf(copyAppsCommand,sizeof(copyAppsCommand),"cp ./apps/* %s/.simplemenu/apps",home);
		ret = system(copyAppsCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyGamesCommand[5000];
		mkdir(pathToGameFiles,0700);
		snprintf(copyGamesCommand,sizeof(copyGamesCommand),"cp ./games/* %s/.simplemenu/games",home);
		ret = system(copyGamesCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copyThemesCommand[5000];
		mkdir(pathToThemeFiles,0700);
		snprintf(copyThemesCommand,sizeof(copyThemesCommand),"cp -r ./themes/* %s/.simplemenu/themes",home);
		ret = system(copyThemesCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char copySectionGroupsCommand[5000];
		mkdir(pathToSectionGroupsFiles,0700);
		snprintf(copySectionGroupsCommand,sizeof(copySectionGroupsCommand),"cp -r ./section_groups/* %s/.simplemenu/section_groups",home);
		ret = system(copySectionGroupsCommand);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
	}
	mkdir(pathToTempFiles,0700);
}

void saveFavorites() {
	if (favoritesChanged) {
		FILE * fp;
		char pathToFavoritesFilePlusFileName[300];
		snprintf(pathToFavoritesFilePlusFileName,sizeof(pathToFavoritesFilePlusFileName),"%s/.simplemenu/favorites.sav",home);
		fp = fopen(pathToFavoritesFilePlusFileName, "w");
		int linesWritten=0;
		for (int j=0;j<favoritesSize;j++) {
			struct Favorite favorite = favorites[j];
			if (strlen(favorite.name)==1) {
				break;
			}
			if(linesWritten>0) {
				fprintf(fp,"\n");
			}
			fprintf(fp,"%s;",favorite.section);
			fprintf(fp,"%s;",favorite.name);
			if(favorite.alias[0]=='\0') {
				fprintf(fp," ;");
			} else {
				fprintf(fp,"%s;",favorite.alias);
			}
			fprintf(fp,"%s;",favorite.emulatorFolder);
			fprintf(fp,"%s;",favorite.executable);
			fprintf(fp,"%s",favorite.filesDirectory);
			linesWritten++;
		}
		fclose(fp);
		favoritesChanged=0;
	}
}

void loadFavorites() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char pathToFavoritesFilePlusFileName[300];
	snprintf(pathToFavoritesFilePlusFileName,sizeof(pathToFavoritesFilePlusFileName),"%s/.simplemenu/favorites.sav",home);
	fp = fopen(pathToFavoritesFilePlusFileName, "r");
	if (fp==NULL) {
		generateError("FAVORITES FILE NOT FOUND-SHUTTING DOWN",1);
		return;
	}
	char *configurations[6];
	char *ptr;
	favoritesSize=0;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		strcpy(favorites[favoritesSize].section,configurations[0]);
		strcpy(favorites[favoritesSize].name,configurations[1]);
		strcpy(favorites[favoritesSize].alias,configurations[2]);
		strcpy(favorites[favoritesSize].emulatorFolder,configurations[3]);
		strcpy(favorites[favoritesSize].executable,configurations[4]);
		strcpy(favorites[favoritesSize].filesDirectory,configurations[5]);
		int len = strlen(favorites[favoritesSize].filesDirectory);
		if (favorites[favoritesSize].filesDirectory[len-1]=='\n') {
			favorites[favoritesSize].filesDirectory[len-1]='\0';
		}
		favoritesSize++;
	}
	fclose(fp);
	if (line) {
		free(line);
	}
	qsort(favorites, favoritesSize, sizeof(struct Favorite), compareFavorites);
}

int cmpfnc(const void *f1, const void *f2)
{
	struct SectionGroup *e1 = (struct SectionGroup *)f1;
	struct SectionGroup *e2 = (struct SectionGroup *)f2;
	char temp1[300]="";
	char temp2[300]="";
	strcpy(temp1,e1->groupName);
	strcpy(temp2,e2->groupName);
	for(int i=0;temp1[i]; i++) {
		temp1[i] = tolower(temp1[i]);
	}
	for(int i=0;temp2[i]; i++) {
		temp2[i] = tolower(temp2[i]);
	}
	return strcmp(temp1, temp2);
}


void loadConfig() {
	char pathToConfigFilePlusFileName[1000];
	const char *value;
	snprintf(pathToConfigFilePlusFileName,sizeof(pathToConfigFilePlusFileName),"%s/.simplemenu/config.ini",home);
	ini_t *config = ini_load(pathToConfigFilePlusFileName);

	value = ini_get(config, "GENERAL", "media_folder");
	strcpy(mediaFolder,value);

	value = ini_get(config, "GENERAL", "logging_enabled");

	if (atoifgl(value)==1) {
		enableLogging();
	}

	value = ini_get(config, "CPU", "underclocked_speed");
	OC_UC=atoifgl(value);

	value = ini_get(config, "CPU", "normal_speed");
	OC_NO=atoifgl(value);

	value = ini_get(config, "CPU", "overclocked_speed");
	OC_OC=atoifgl(value);

	value = ini_get(config, "CPU", "sleep_speed");
	OC_SLEEP=atoifgl(value);

	value = ini_get(config, "SCREEN", "width");
	SCREEN_WIDTH=atoifgl(value);

	value = ini_get(config, "SCREEN", "height");
	SCREEN_HEIGHT=atoifgl(value);

	value = ini_get(config, "CONTROLS", "A");
	if (value) {
		BTN_A = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "B");
	if (value) {
		BTN_B = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "X");
	if (value) {
		BTN_X = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "Y");
	if (value) {
		BTN_Y = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "L1");
	if (value) {
		BTN_L1 = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "L2");
	if (value) {
		BTN_L2 = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "R1");
	if (value) {
		BTN_R1 = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "R2");
	if (value) {
		BTN_R2 = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "UP");
	if (value) {
		BTN_UP = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "DOWN");
	if (value) {
		BTN_DOWN = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "LEFT");
	if (value) {
		BTN_LEFT = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "RIGHT");
	if (value) {
		BTN_RIGHT = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "START");
	if (value) {
		BTN_START = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "SELECT");
	if (value) {
		BTN_SELECT = atoifgl(value);
	}

	value = ini_get(config, "CONTROLS", "R");
	if (value) {
		BTN_R = atoifgl(value);
	}

	ini_free(config);
}

void loadSectionGroups() {
	ini_t *themeConfig = ini_load(pathToThemeConfigFilePlusFileName);
	sectionGroupCounter=0;
	char *files[1000];
	char tempString[1000];

	setThemeResourceValueInSection (themeConfig, "GENERAL", "section_groups_folder", sectionGroupsFolder);

	if (strlen(sectionGroupsFolder)>1) {
		snprintf(tempString,sizeof(tempString),"%s",sectionGroupsFolder);
	} else {
		snprintf(tempString,sizeof(tempString),"%s/.simplemenu/section_groups/",getenv("HOME"));
	}

	int n = recursivelyScanDirectory(tempString, files, 0);

	for(int i=0;i<n;i++) {
		if(strstr(files[i],".png")!=NULL) {
			continue;
		}
		strcpy(sectionGroups[sectionGroupCounter].groupPath, files[i]);

		char *temp3 = malloc(3000);
		char *temp = getNameWithoutPath((files[i]));
		char *temp1 = getNameWithoutExtension(temp);
		char *sectionGroupPath = getRomPath(files[i]);

		if (strlen(sectionGroupsFolder)>1) {
			strcpy(temp3,sectionGroupsFolder);
		} else {
			strcpy(temp3,sectionGroupPath);
			strcat(temp3,"/");
		}
		strcat(temp3,temp1);
		strcat(temp3,".png");

		strcpy(sectionGroups[sectionGroupCounter].groupBackground, temp3);
		logMessage("INFO","Loading group background");
		sectionGroups[sectionGroupCounter].groupBackgroundSurface=IMG_Load(sectionGroups[sectionGroupCounter].groupBackground);
		resizeGroupBackground(&sectionGroups[sectionGroupCounter]);

		char *temp2 = toUpper(temp1);
		strcpy(sectionGroups[sectionGroupCounter].groupName, temp2);
		free(temp);
		free(temp1);
		free(temp2);
		free(temp3);
		free(sectionGroupPath);
		sectionGroupCounter++;
	}

	for (int i=0;i<n;i++){
		free(files[i]);
	}

	qsort(sectionGroups, sectionGroupCounter, sizeof(struct SectionGroup), cmpfnc);
	ini_free(themeConfig);
}

int loadSections(char *file) {
	menuSectionCounter=0;
	char pathToSectionsFilePlusFileName[1000];
	snprintf(pathToSectionsFilePlusFileName,sizeof(pathToSectionsFilePlusFileName),"%s",file);
	ini_t *config = ini_load(pathToSectionsFilePlusFileName);
	ini_t *themeConfig = ini_load(pathToThemeConfigFilePlusFileName);

	const char *consoles = ini_get(config, "CONSOLES", "consoleList");
	char *consoles1 = strdup(consoles);
	char *sectionNames[10000];
	const char *value;
	int sectionCounter=0;

	char* tokenizedSectionName=strtok(consoles1,",");

	while(tokenizedSectionName!=NULL) {
		value = ini_get(config, tokenizedSectionName, "execs");
		if(value!=NULL) {
			sectionNames[sectionCounter]=malloc(strlen(tokenizedSectionName)+1);
			strcpy(sectionNames[sectionCounter], tokenizedSectionName);
			sectionCounter++;
		}
		tokenizedSectionName=strtok(NULL,",");
	}
	free(tokenizedSectionName);
	free(consoles1);
	while(menuSectionCounter<sectionCounter) {
		char *sectionName = sectionNames[menuSectionCounter];
		logMessage("INFO",sectionName);
		strcpy(menuSections[menuSectionCounter].sectionName, sectionName);

		//READ EXECS
		int execCounter=0;
		value = ini_get(config, sectionName, "execs");
		char *value2 = malloc(3000);
		strcpy(value2,value);
		char* currentExec = strtok(value2,",");
		while(currentExec!=NULL) {
			#ifndef TARGET_PC
			char *tempNameWithoutPath = getNameWithoutPath(currentExec);
			char *tempPathWithoutName = getRomPath(currentExec);
			#else
			char *tempNameWithoutPath = malloc(strlen(currentExec)+1);
			strcpy(tempNameWithoutPath, currentExec);
			char *tempPathWithoutName = "\0";
			#endif

			menuSections[menuSectionCounter].executables[execCounter]=malloc(strlen(tempNameWithoutPath)+1);
			strcpy(menuSections[menuSectionCounter].executables[execCounter],tempNameWithoutPath);
			free(tempNameWithoutPath);

			menuSections[menuSectionCounter].emulatorDirectories[execCounter]=malloc(strlen(tempPathWithoutName)+2);
			strcpy(menuSections[menuSectionCounter].emulatorDirectories[execCounter],tempPathWithoutName);
			strcat(menuSections[menuSectionCounter].emulatorDirectories[execCounter],"/");

			currentExec = strtok(NULL,",");
			execCounter++;
		}
		logMessage("INFO","Current exec ready");
		free(value2);
		logMessage("INFO","free as a bird");
		for (int i=execCounter;i<10;i++) {
			menuSections[menuSectionCounter].executables[i]=NULL;
			menuSections[menuSectionCounter].emulatorDirectories[i]=NULL;
		}
		logMessage("INFO","free as a bird 2");
		menuSections[menuSectionCounter].activeExecutable=0;
		logMessage("INFO","Executable set");
		setStringValueInSection (config, sectionName, "romDirs", menuSections[menuSectionCounter].filesDirectories,"\0");
		setStringValueInSection (config, sectionName, "romExts", menuSections[menuSectionCounter].fileExtensions,"\0");
		setStringValueInSection (config, sectionName, "scaling", menuSections[menuSectionCounter].scaling,"3");
		setRGBColorInSection(themeConfig, sectionName, "headerBackGround", menuSections[menuSectionCounter].headerAndFooterBackgroundColor);
		setRGBColorInSection(themeConfig, sectionName, "headerFont", menuSections[menuSectionCounter].headerAndFooterTextColor);
		setRGBColorInSection(themeConfig, sectionName, "bodyBackground", menuSections[menuSectionCounter].bodyBackgroundColor);
		setRGBColorInSection(themeConfig, sectionName, "bodyFont", menuSections[menuSectionCounter].bodyTextColor);
		setRGBColorInSection(themeConfig, sectionName, "selectedItemBackground", menuSections[menuSectionCounter].bodySelectedTextBackgroundColor);
		setRGBColorInSection(themeConfig, sectionName, "selectedItemFont", menuSections[menuSectionCounter].bodySelectedTextTextColor);
		setRGBColorInSection(themeConfig, menuSections[menuSectionCounter].sectionName, "pictureTextFont", menuSections[menuSectionCounter].pictureTextColor);
		if (menuSections[menuSectionCounter].systemLogoSurface!=NULL) {
			SDL_FreeSurface(menuSections[menuSectionCounter].systemLogoSurface);
			menuSections[menuSectionCounter].systemLogoSurface = NULL;
			SDL_FreeSurface(menuSections[menuSectionCounter].systemPictureSurface);
			menuSections[menuSectionCounter].systemPictureSurface = NULL;
			SDL_FreeSurface(menuSections[menuSectionCounter].backgroundSurface);
			menuSections[menuSectionCounter].backgroundSurface = NULL;
		}
		logMessage("INFO","Setting logo, background and system");
		setThemeResourceValueInSection (themeConfig, sectionName, "logo", menuSections[menuSectionCounter].systemLogo);
		setThemeResourceValueInSection (themeConfig, sectionName, "background", menuSections[menuSectionCounter].background);
		setThemeResourceValueInSection (themeConfig, sectionName, "system", menuSections[menuSectionCounter].systemPicture);

		value = ini_get(themeConfig, "GENERAL", "system_w_in_custom");
		systemWidth = atoifgl(value);

		value = ini_get(themeConfig, "GENERAL", "system_h_in_custom");
		systemHeight = atoifgl(value);

		if (menuSectionCounter==currentSectionNumber) {
			logMessage("INFO","load sections - Loading system logo");
			menuSections[menuSectionCounter].systemLogoSurface = IMG_Load(menuSections[menuSectionCounter].systemLogo);
			resizeSectionSystemLogo(&menuSections[menuSectionCounter]);
			logMessage("INFO","Loading system background");
			menuSections[menuSectionCounter].backgroundSurface = IMG_Load(menuSections[menuSectionCounter].background);
			resizeSectionBackground(&menuSections[menuSectionCounter]);
			logMessage("INFO","Loading system picture");
			menuSections[menuSectionCounter].systemPictureSurface = IMG_Load(menuSections[menuSectionCounter].systemPicture);
			resizeSectionSystemPicture(&menuSections[menuSectionCounter]);
		}

		logMessage("INFO","Set");

		value = ini_get(config, sectionName, "aliasFile");
		if(value!=NULL) {
			strcpy(menuSections[menuSectionCounter].aliasFileName,value);
		} else {
			strcpy(menuSections[menuSectionCounter].aliasFileName,"\0");
		}

		setStringValueInSection (config, sectionName, "category", menuSections[menuSectionCounter].category,"\0");

		value = ini_get(config, sectionName, "onlyFileNamesNoPathOrExtension");
		if(value!=NULL&&strcmp("yes",value)==0) {
			menuSections[menuSectionCounter].onlyFileNamesNoExtension=1;
		}
		menuSections[menuSectionCounter].currentPage=0;
		menuSections[menuSectionCounter].currentGameInPage=0;
		menuSectionCounter++;
		free(sectionName);
		logMessage("INFO","Out");
	}
	value = ini_get(themeConfig, "GENERAL", "colorful_fullscreen_menu");
	if (value == NULL) {
		colorfulFullscreenMenu = 0;
	} else {
		colorfulFullscreenMenu = atoifgl(value);
	}

	value = ini_get(themeConfig, "GENERAL", "font_outline");
	if (value == NULL) {
		fontOutline = 0;
	} else {
		fontOutline = atoifgl(value);
	}

	value = ini_get(themeConfig, "GENERAL", "display_section_group_name");
	if (value == NULL) {
		displaySectionGroupName = 0;
	} else {
		displaySectionGroupName = atoifgl(value);
	}

	value = ini_get(themeConfig, "GENERAL", "items_in_custom");
	itemsPerPage = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "items_separation_in_custom");
	itemsSeparation = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "items_in_full_custom");
	itemsPerPageFullscreen = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "game_list_alignment_in_custom");
	gameListAlignment = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "game_list_x_in_custom");
	gameListX = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "game_list_y_in_custom");
	gameListY = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "game_list_w_in_custom");
	gameListWidth = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "game_list_position_in_full_custom");
	gameListPositionFullScreen = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_max_w_in_custom");
	artWidth = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_max_h_in_custom");
	artHeight = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_x_in_custom");
	artX = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_y_in_custom");
	artY = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "system_x_in_custom");
	systemX = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "system_y_in_custom");
	systemY = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "font_size_custom");
	fontSize = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "text1_font_size_in_custom");
	text1FontSize = atoi (value);

	value = ini_get(themeConfig, "GENERAL", "text1_x_in_custom");
	text1X = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "text1_y_in_custom");
	text1Y = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "text1_alignment_in_custom");
	text1Alignment = atoifgl(value);

	setThemeResourceValueInSection (themeConfig, "GENERAL", "textX_font_custom", textXFont);
	value = ini_get(themeConfig, "GENERAL", "text2_font_size_in_custom");

	text2FontSize = atoi (value);

	value = ini_get(themeConfig, "GENERAL", "text2_x_in_custom");
	text2X = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "text2_y_in_custom");
	text2Y = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "text2_alignment_in_custom");
	text2Alignment = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_text_distance_from_picture_in_custom");
	artTextDistanceFromPicture = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_text_line_separation_in_custom");
	artTextLineSeparation = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "art_text_font_size_in_custom");
	artTextFontSize = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "font_size");
	baseFont = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "transparent_shading");
	transparentShading  = atoifgl(value);

	value = ini_get(themeConfig, "GENERAL", "fullscreen_footer_on_top");
	footerOnTop = atoifgl(value);

	setThemeResourceValueInSection (themeConfig, "GENERAL", "favorite_indicator", favoriteIndicator);
	setThemeResourceValueInSection (themeConfig, "GENERAL", "font", menuFont);
	setThemeResourceValueInSection (themeConfig, "GENERAL", "section_groups_folder", sectionGroupsFolder);

	freeFonts();
	initializeFonts();

	strcpy(menuSections[menuSectionCounter].sectionName,"FAVORITES");
	menuSections[menuSectionCounter].emulatorDirectories[0]=strdup("/some/folder/");
	menuSections[menuSectionCounter].activeEmulatorDirectory=0;
	menuSections[menuSectionCounter].executables[0]=NULL;
	menuSections[menuSectionCounter].activeExecutable=0;
	strcpy(menuSections[menuSectionCounter].filesDirectories,"/some/folder");
	strcpy(menuSections[menuSectionCounter].fileExtensions,".zzz");
	strcpy(menuSections[menuSectionCounter].category, "all");
	menuSections[menuSectionCounter].onlyFileNamesNoExtension=0;
	menuSections[menuSectionCounter].hidden=0;
	menuSections[menuSectionCounter].currentPage=0;
	menuSections[menuSectionCounter].currentGameInPage=0;
	setRGBColorInSection(themeConfig, "FAVORITES", "headerBackGround", menuSections[menuSectionCounter].headerAndFooterBackgroundColor);
	setRGBColorInSection(themeConfig, "FAVORITES", "headerFont", menuSections[menuSectionCounter].headerAndFooterTextColor);
	setRGBColorInSection(themeConfig, "FAVORITES", "bodyBackground", menuSections[menuSectionCounter].bodyBackgroundColor);
	setRGBColorInSection(themeConfig, "FAVORITES", "bodyFont", menuSections[menuSectionCounter].bodyTextColor);
	setRGBColorInSection(themeConfig, "FAVORITES", "selectedItemBackground", menuSections[menuSectionCounter].bodySelectedTextBackgroundColor);
	setRGBColorInSection(themeConfig, "FAVORITES", "selectedItemFont", menuSections[menuSectionCounter].bodySelectedTextTextColor);
	setRGBColorInSection(themeConfig, menuSections[menuSectionCounter].sectionName, "pictureTextFont", menuSections[menuSectionCounter].pictureTextColor);
	setThemeResourceValueInSection (themeConfig, "FAVORITES", "logo", menuSections[menuSectionCounter].systemLogo);
	setThemeResourceValueInSection (themeConfig, "FAVORITES", "system", menuSections[menuSectionCounter].systemPicture);
	setThemeResourceValueInSection (themeConfig, "FAVORITES", "background", menuSections[menuSectionCounter].background);
	menuSectionCounter++;
	favoritesSectionNumber=menuSectionCounter-1;
	ini_free(config);
	ini_free(themeConfig);
	return menuSectionCounter;
}

int countSections(char *file) {
	char pathToSectionsFilePlusFileName[1000];
	snprintf(pathToSectionsFilePlusFileName,sizeof(pathToSectionsFilePlusFileName),"%s",file);
	ini_t *config = ini_load(pathToSectionsFilePlusFileName);

	const char *consoles = ini_get(config, "CONSOLES", "consoleList");
	char *consoles1 = strdup(consoles);
	const char *value;
	int sectionCounter=0;

	char* tokenizedSectionName=strtok(consoles1,",");

	while(tokenizedSectionName!=NULL) {
		value = ini_get(config, tokenizedSectionName, "execs");
		if(value!=NULL) {
			sectionCounter++;
		}
		tokenizedSectionName=strtok(NULL,",");
	}
	free(consoles1);
	ini_free(config);
	return sectionCounter;
}

void saveLastState() {
	FILE * fp;
	char pathToStatesFilePlusFileName[300];
	snprintf(pathToStatesFilePlusFileName,sizeof(pathToStatesFilePlusFileName),"%s/.simplemenu/last_state.sav",home);
	fp = fopen(pathToStatesFilePlusFileName, "w");
	fprintf(fp, "%d;\n", 61);
	fprintf(fp, "%d;\n", stripGames);
	fprintf(fp, "%d;\n", fullscreenMode);
	fprintf(fp, "%d;\n", footerVisibleInFullscreenMode);
	fprintf(fp, "%d;\n", menuVisibleInFullscreenMode);
	fprintf(fp, "%d;\n", activeTheme);
	fprintf(fp, "%d;\n", timeoutValue);
	fprintf(fp, "%d;\n", autoHideLogos);
	fprintf(fp, "%d;\n", activeGroup);
	fprintf(fp, "%d;\n", currentSectionNumber);
	fprintf(fp, "%d;\n", currentMode);
	for(int groupCount=0;groupCount<sectionGroupCounter;groupCount++) {
		int sectionsNum=countSections(sectionGroups[groupCount].groupPath);
		for (int sectionCount=0;sectionCount<=sectionsNum;sectionCount++) {
			if (groupCount==activeGroup) {
				int isActive = 0;
				if (sectionCount==currentSectionNumber) {
					isActive=1;
				}
				fprintf(fp, "%d;%d;%d;%d;%d;%d\n", isActive, sectionCount, menuSections[sectionCount].currentPage, menuSections[sectionCount].currentGameInPage, menuSections[sectionCount].realCurrentGameNumber, returnTo);
			} else {
				fprintf(fp, "%d;%d;%d;%d;%d;%d\n", sectionGroupStates[groupCount][sectionCount][0], sectionCount, sectionGroupStates[groupCount][sectionCount][1], sectionGroupStates[groupCount][sectionCount][2], sectionGroupStates[groupCount][sectionCount][3], returnTo);
			}
		}
	}
	fclose(fp);
}

void loadLastState() {
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	char pathToStatesFilePlusFileName[300];
	snprintf(pathToStatesFilePlusFileName,sizeof(pathToStatesFilePlusFileName),"%s/.simplemenu/last_state.sav",home);

	fp = fopen(pathToStatesFilePlusFileName, "r");
	if (fp==NULL) {
		saveLastState();
		return;
	}
	char *configurations[6];
	char *ptr;
	int startInSection = -1;
	int startInPictureMode = -1;
	int stripGamesConfig = -1;
	int startInGroup= -1;
	int footerVisible= -1;
	int menuVisible= -1;
	int themeRead= -1;
	int timeout= -1;
	int readAutoHideLogos= -1;
	int groupCounter=-1;
	int savedVersion=-1;
	int itemsRead=-1;
	while ((read = getline(&line, &len, fp)) != -1) {
		ptr = strtok(line, ";");
		int i=0;
		while(ptr != NULL) {
			configurations[i]=ptr;
			ptr = strtok(NULL, ";");
			i++;
		}
		if (savedVersion==-1) {
			savedVersion=atoifgl(configurations[0]);
			if(savedVersion!=61) {
				saveLastState();
				fclose(fp);
				if (line) {
					free(line);
				}
				return;
			}
		} else if (stripGamesConfig==-1) {
			stripGamesConfig=atoifgl(configurations[0]);
		} else if (startInPictureMode==-1){
			startInPictureMode=atoifgl(configurations[0]);
		} else if(footerVisible==-1) {
			footerVisible=atoifgl(configurations[0]);
		} else if(menuVisible==-1) {
			menuVisible=atoifgl(configurations[0]);
		} else if(themeRead==-1) {
			themeRead=atoifgl(configurations[0]);
		} else if(timeout==-1) {
			timeout=atoifgl(configurations[0]);
		} else if(readAutoHideLogos==-1) {
			readAutoHideLogos=atoifgl(configurations[0]);
		} else if (startInGroup==-1) {
			startInGroup = atoifgl(configurations[0]);
		} else if (startInSection==-1) {
			startInSection=atoifgl(configurations[0]);
		} else if (itemsRead==-1) {
			itemsRead=atoifgl(configurations[0]);
		}
		else {
			if(atoifgl(configurations[1])==0) {
				groupCounter++;
			}
			int isActive =atoifgl(configurations[0]);
			int sectionNumber =atoifgl(configurations[1]);
			int page = atoifgl(configurations[2]);
			int game = atoifgl(configurations[3]);
			int realCurrentGameNumber = atoifgl(configurations[4]);
			int retTo = atoifgl(configurations[5]);
			sectionGroupStates[groupCounter][sectionNumber][0]=isActive;
			sectionGroupStates[groupCounter][sectionNumber][1]=page;
			sectionGroupStates[groupCounter][sectionNumber][2]=game;
			sectionGroupStates[groupCounter][sectionNumber][3]=realCurrentGameNumber;
			sectionGroupStates[groupCounter][sectionNumber][4]=retTo;
			if (groupCounter==startInGroup) {
				menuSections[sectionNumber].currentPage=page;
				menuSections[sectionNumber].currentGameInPage=game;
				menuSections[sectionNumber].realCurrentGameNumber=realCurrentGameNumber;
				returnTo=retTo;
			}
			menuSections[sectionNumber].alphabeticalPaging=0;
		}
	}
	stripGames=stripGamesConfig;
	fullscreenMode=startInPictureMode;
	footerVisibleInFullscreenMode=footerVisible;
	menuVisibleInFullscreenMode=menuVisible;
	activeTheme=themeRead;
	timeoutValue=timeout;
	autoHideLogos=readAutoHideLogos;
	currentSectionNumber=startInSection;
	activeGroup = startInGroup;
	currentMode=itemsRead;
	fclose(fp);
	if (line) {
		free(line);
	}
}
