#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include <sys/ioctl.h>
#if defined(TARGET_NPG) || defined(TARGET_OD) || defined TARGET_OD_BETA
#include <linux/vt.h>
#endif
#include <fcntl.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/constants.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/graphics.h"
#include "../headers/screen.h"
#include "../headers/string_utils.h"
#include "../headers/system_logic.h"
#include "../headers/opk.h"
#include "../headers/input.h"
#include "../headers/doubly_linked_rom_list.h"
#include "../headers/utils.h"


FILE *getCurrentSectionAliasFile() {
	FILE *aliasFile;
	aliasFile = fopen(CURRENT_SECTION.aliasFileName, "r");
	return aliasFile;
}

struct Favorite findFavorite(char *name) {
	struct Favorite favorite;
	for (int i=0; i<favoritesSize;i++) {
		favorite = favorites[i];
		if (strcmp(favorite.name, name)==0) {
			return favorite;
		}
	}
	return favorite;
}

char *search(char* arr[], int n, char *x)
{

	if (strcmp(arr[n - 1],x)==0)
		return "Found";

	char *backup = arr[n - 1];
	arr[n - 1] = x;

	for (int i = 0;; i++) {
		if (strcmp(arr[i],x)==0) {
			arr[n - 1] = backup;
			if (i < n - 1)
				return "Found";
			return "Not Found";
		}
	}
	return "Not Found";
}

char *getRomRealName(char *romName) {
	char *nameTakenFromAlias;
	char* strippedNameWithoutExtension = malloc(strlen(romName)+1);
	strcpy(strippedNameWithoutExtension,romName);
	stripGameName(strippedNameWithoutExtension);
	nameTakenFromAlias=ht_get(CURRENT_SECTION.aliasHashTable,strippedNameWithoutExtension);
	if (nameTakenFromAlias!=NULL) {
		return(nameTakenFromAlias);
	} else {
		char *dup=strdup(romName);
		stripGameName(dup);
		return(dup);
	}
	if(strippedNameWithoutExtension != NULL) {
		free(strippedNameWithoutExtension);
	}
	nameTakenFromAlias=malloc(strlen(romName)+1);
	strcpy(nameTakenFromAlias, romName);
	return(nameTakenFromAlias);
}

int getOPK(char *package_path, struct OPKDesktopFile* desktopFiles) {
	struct OPK *opk = opk_open(package_path);
	if (opk==NULL) {
		return 0;
	}
	char *name;
	char *category;
	char *terminal;

	int i=0;
	while (1) {
		const char *metadata_name;
		if (opk_open_metadata(opk, &metadata_name) <= 0) {
			break;
		}
		strcpy(desktopFiles[i].parentOPK,package_path);
		strcpy(desktopFiles[i].name,metadata_name);
		const char *key=NULL;
		const char *val=NULL;
		size_t skey, sval;
		while(opk_read_pair(opk, &key, &skey, &val, &sval) && key) {
			if (!strncmp(key, "Name", skey)) {
				name=malloc((int)sval+1);
				strncpy(name,val,(int)sval);
				name[sval]='\0';
				strcpy(desktopFiles[i].displayName,name);
				free(name);
			}
			if (!strncmp(key, "Categories", skey)) {
				category=malloc((int)sval+1);
				strncpy(category,val,(int)sval);
				category[sval]='\0';
				strcpy(desktopFiles[i].category,category);
				free(category);
			}
			if (!strncmp(key, "Terminal", skey)) {
				terminal=malloc((int)sval+1);
				strncpy(terminal,val,(int)sval);
				terminal[sval]='\0';
				desktopFiles[i].isConsoleApp=strcmp(terminal,"false");
				free(terminal);
			}
		}
		i++;
	}
	opk_close(opk);
	return i;
}

char *getAlias(char *romName) {
	char *alias = malloc(300);
	if (strlen(CURRENT_SECTION.aliasFileName)>1) {
		strcpy(alias, getRomRealName(romName));
	} else {
		strcpy(alias, romName);
	}
	if(strcmp(alias, romName)==0) {
		if(currentSectionNumber==favoritesSectionNumber) {
			struct Favorite favorite = findFavorite(alias);
			if (strlen(favorite.alias)<2) {
			}
		} else {
			char *ext = getExtension(romName);
			if (strcmp(ext,".opk")==0) {
				strcpy (alias, romName);
			}
		}
	}
	return alias;
}

char *getFileNameOrAlias(struct Rom *rom) {
	char *displayName = malloc(300);
	if(rom->alias!=NULL&&strlen(rom->alias)>2) {
		//this is a favorite with no alias
		if(strcmp(rom->alias, " ")!=0) {
			strcpy(displayName, rom->alias);
		} else {
			strcpy(displayName, rom->name);
		}
	} else {
		strcpy(displayName, rom->name);
	}
	if(strcmp(displayName, rom->name)==0) {
		char *dup = strdup(displayName);
		free(displayName);
		stripGameName(dup);
		displayName = strdup(dup);
		free(dup);
	}
	return displayName;
}

void generateError(char *pErrorMessage, int pThereIsACriticalError) {
	itsStoppedBecauseOfAnError=1;
	errorMessage=pErrorMessage;
	thereIsACriticalError=pThereIsACriticalError;
}

void quit() {
	saveLastState();
	saveFavorites();
	clearTimer();
	clearPicModeHideLogoTimer();
	clearPicModeHideMenuTimer();
	freeResources();
	if (shutDownEnabled) {
#ifdef TARGET_PC
		exit(0);
#endif
		if (selectedShutDownOption==1) {
			execlp("sh", "sh", "-c", "sync && reboot", NULL);
		} else {
			execlp("sh", "sh", "-c", "sync && poweroff", NULL);
		}
	} else {
		if (selectedShutDownOption==1) {
			execlp("sh", "sh", "-c", "sync && reboot", NULL);
		} else if (selectedShutDownOption==2) {
			execlp("sh", "sh", "-c", "sync && poweroff", NULL);
		} else {
			exit(0);
		}
	}
}

int doesFavoriteExist(char *name) {
	for(int i=0;i<favoritesSize;i++) {
		if (strstr(name,favorites[i].name)) {
			return 1;
		}
	}
	return 0;
}

void setSectionsState(char *states) {
	char *endSemiColonStr;
	char *semiColonToken = strtok_r(states, ";", &endSemiColonStr);
	int i=0;
	while (semiColonToken != NULL)
	{
		char *endDashToken;
		char *dashToken = strtok_r(semiColonToken, "-", &endDashToken);
		int j=0;
		while (dashToken != NULL) {
			if (j==0) {
				menuSections[i].currentPage=atoi(dashToken);
			} else if (j==1) {
				menuSections[i].currentGameInPage=atoi(dashToken);
			} else {
				menuSections[i].alphabeticalPaging=atoi(dashToken);
			}
			j++;
			dashToken = strtok_r(NULL, "-", &endDashToken);
		}
		semiColonToken = strtok_r(NULL, ";", &endSemiColonStr);
		i++;
	}
}

int checkIfEmulatorExists(char *path, char *executable) {
	char executableWithFullPath[300];
	strcpy(executableWithFullPath,path);
	strcat(executableWithFullPath,executable);
	FILE * fp;
	fp = fopen(executableWithFullPath, "r");
	if (fp==NULL) {
		return 0;
	}
	fclose(fp);
	return 1;
}

void resetFrameBuffer1 () {
	int ret = system("./scripts/reset_fb");
	if (ret==-1) {
		generateError("FATAL ERROR", 1);
	}
}

#ifndef TARGET_PC
void executeCommand (char *emulatorFolder, char *executable, char *fileToBeExecutedWithFullPath, int consoleApp) {
#else
	void executeCommandPC (char *executable, char *fileToBeExecutedWithFullPath, int consoleApp) {
#endif
		char *exec = malloc(strlen(executable)+5000);
		strcpy(exec, executable);
		//#ifndef TARGET_OD_BETA
		unsetenv("SDL_FBCON_DONT_CLEAR");
		//#endif
		char states[2000];
		//		for (int i=0;i<favoritesSectionNumber+1;i++) {
		//			char tempString[200];
		//			snprintf(tempString,sizeof(tempString),"%d-%d-%d;",menuSections[i].currentPage,menuSections[i].currentGameInPage,menuSections[i].alphabeticalPaging);
		//			strcat(states,tempString);
		//		}
		char pReturnTo[3];
		snprintf(pReturnTo,sizeof(pReturnTo),"%d;",returnTo);
		char pSectionNumber[3]="";
		char pPictureMode[2]="";
		snprintf(pSectionNumber,sizeof(pSectionNumber),"%d",currentSectionNumber);
		snprintf(pPictureMode,sizeof(pPictureMode),"%d",fullscreenMode);
		saveLastState();
#ifndef TARGET_PC
		saveFavorites();
		clearTimer();
		clearPicModeHideLogoTimer();
		clearPicModeHideMenuTimer();
#endif
		logMessage("INFO", "Launching Game");
#ifndef TARGET_PC
		setCPU(currentCPU);
#endif
#ifdef TARGET_RFW
		//	ipu modes (/proc/jz/ipu):
		//	0: stretch
		//	1: aspect
		//	2: original (fallback to aspect when downscale is needed)
		//	3: 4:3
		FILE *fp = fopen("/proc/jz/ipu","w");
		fprintf(fp,CURRENT_SECTION.scaling);
		fclose(fp);
#endif
#ifdef TARGET_OD_BETA
		if (strcmp(CURRENT_SECTION.scaling,"0")==0) { //0: stretch
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_MODE=0");
		} else if (strcmp(CURRENT_SECTION.scaling,"1")==0) { //1: aspect
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_MODE=1");
		} else if (strcmp(CURRENT_SECTION.scaling,"2")==0) {
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_MODE=2"); //2: integer scaling
		} else if (strcmp(CURRENT_SECTION.scaling,"3")==0) {
			SDL_putenv("SDL_VIDEO_KMSDRM_SCALING_MODE=3"); //2: integer scaling
		}
#endif
#ifndef TARGET_PC
		logMessage("INFO",exec);
		logMessage("INFO",fileToBeExecutedWithFullPath);
		SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		SDL_ShowCursor(1);
		freeResources();
		SDL_ShowCursor(1);
#ifndef TARGET_OD_BETA
		resetFrameBuffer1();
#endif

		if (consoleApp) {
#if defined(TARGET_NPG) || defined(TARGET_OD) || defined TARGET_OD_BETA
			/* Enable the framebuffer console */
			char c = '1';
			int fd = open("/sys/devices/virtual/vtconsole/vtcon1/bind", O_WRONLY);
			if (fd < 0) {
				printf("Unable to open fbcon handle\n");
			} else {
				write(fd, &c, 1);
				close(fd);
			}

			fd = open("/dev/tty1", O_RDWR);
			if (fd < 0) {
				printf("Unable to open tty1 handle\n");
			} else {
				if (ioctl(fd, VT_ACTIVATE, 1) < 0)
					printf("Unable to activate tty1\n");
				close(fd);
			}
#endif
		}
		execlp("./invoker.dge","invoker.dge", emulatorFolder, exec, fileToBeExecutedWithFullPath, states, pSectionNumber, pReturnTo, pPictureMode, NULL);
#else
		strcat(exec, " \"");
		strcat(exec, fileToBeExecutedWithFullPath);
		strcat(exec, "\"");
		logMessage("INFO",exec);
		freeResources();
		int ret = system(exec);
		if(ret == -1) {
			logMessage("ERROR","Error executing emulator");
		}
		free(exec);
		initializeDisplay();
		setupDisplayAndKeys();
		enableKeyRepeat();
		initializeFonts();
		initializeSettingsFonts();
#endif
	}

	int isExtensionValid(char *extension, char *fileExtensions) {
		char fileExtensionsCopy[200];
		strcpy(fileExtensionsCopy, fileExtensions);
		char *ptr = strtok(fileExtensionsCopy, ",");
		while(ptr != NULL) {
			int areStringsDifferent = strcmp(extension,ptr);
			if (!areStringsDifferent) {
				return(1);
			}
			ptr = strtok(NULL, ",");
		}
		return(0);
	}

	int countGamesInSection() {
		int gamesCounter=0;
		struct Node* currentGameNode = CURRENT_SECTION.head;
		for (int i=0;i<=MAX_GAMES_IN_SECTION;i++) {
			if (currentGameNode!=NULL) {
				gamesCounter++;
				currentGameNode = currentGameNode->next;
			} else {
				break;
			}
		}
		return gamesCounter;
	}

	struct Node *split(struct Node *head)
	{
		struct Node *fast = head,*slow = head;
		while (fast->next && fast->next->next)
		{
			fast = fast->next->next;
			slow = slow->next;
		}
		struct Node *temp = slow->next;
		slow->next = NULL;
		return temp;
	}

	struct Node *merge(struct Node *first, struct Node *second)
	{
		if (!first) {
			return second;
		}

		if (!second) {
			return first;
		}


		char s1Alias[300];
		if(first->data->alias!=NULL&&strlen(first->data->alias)>2) {
			strcpy(s1Alias,first->data->alias);
		} else {
			strcpy(s1Alias,first->data->name);
		}
		char s2Alias[300];
		if(second->data->alias!=NULL&&strlen(second->data->alias)>2) {
			strcpy(s2Alias,second->data->alias);
		} else {
			strcpy(s2Alias,second->data->name);
		}

		char * noPathS1Alias=strdup(s1Alias);
		char * noPathS2Alias=strdup(s2Alias);

		for(char *p = noPathS1Alias;*p;++p) *p=*p>0x40&&*p<0x5b?*p|0x60:*p;
		for(char *p = noPathS2Alias;*p;++p) *p=*p>0x40&&*p<0x5b?*p|0x60:*p;

		if(strlen(CURRENT_SECTION.aliasFileName)<2) {
			stripGameName(noPathS1Alias);
			stripGameName(noPathS2Alias);
		}
		if (strcmp(noPathS1Alias, noPathS2Alias)<=0)
		{
			free(noPathS1Alias);
			free(noPathS2Alias);
			first->next = merge(first->next,second);
			first->next->prev = first;
			first->prev = NULL;
			return first;
		}
		else
		{
			free(noPathS1Alias);
			free(noPathS2Alias);
			second->next = merge(first,second->next);
			second->next->prev = second;
			second->prev = NULL;
			return second;
		}
	}

	struct Node *mergeSort(struct Node *head) {
		if (!head || !head->next) {
			return head;
		}
		struct Node *second = split(head);

		// Recur for left and right halves
		head = mergeSort(head);
		second = mergeSort(second);

		// Merge the two sorted halves
		return merge(head,second);
	}

	void loadFavoritesSectionGameList() {
		int gameInPage = 0;
		int page = 0;
		FAVORITES_SECTION.totalPages=0;
		FAVORITES_SECTION.gameCount=0;
		cleanListForSection(&FAVORITES_SECTION);
		for (int i=0;i<favoritesSize;i++){
			if (gameInPage==ITEMS_PER_PAGE) {
				if(i!=favoritesSize) {
					page++;
					gameInPage = 0;
					FAVORITES_SECTION.totalPages++;
				}
			}

			int size = strlen(favorites[i].name)+1;
			int aliasSize = strlen(favorites[i].alias)+1;

			struct Rom *rom = malloc(sizeof(struct Rom));
			rom->name=malloc(size);
			rom->alias=malloc(aliasSize);
			rom->directory=malloc(1);
			strcpy(rom->alias, favorites[i].alias);
			strcpy(rom->name, favorites[i].name);
			rom->isConsoleApp = favorites[i].isConsoleApp;
			InsertAtTailInSection(&FAVORITES_SECTION, rom);
			gameInPage++;
			FAVORITES_SECTION.gameCount++;
		}
		FAVORITES_SECTION.tail=GetNthNode(FAVORITES_SECTION.gameCount-1);
		if (favoritesSize>0) {
			scrollToGame(FAVORITES_SECTION.realCurrentGameNumber);
		}
	}

	int scanDirectory(char *directory, char* files[], int i)
	{
		struct dirent **result;
		int j=0;
		int n = scandir(directory, &result, NULL, alphasort);
		if (n < 0) {
			logMessage("ERROR: scandir(%s)\n", directory);
			return 0;
		}
		while (j<n) {
			char *d_name = malloc(3000);
			strcpy(d_name, result[j]->d_name);
			if (strcmp(d_name, mediaFolder) != 0 && strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				if (result[j]->d_type & DT_DIR) {
					char path[PATH_MAX];
					char *e = strrchr(d_name, '/');
					if (e==NULL) {
						strcat(d_name, "/");
					}
					free(e);
					snprintf (path, PATH_MAX, "%s%s", directory, d_name);
					//				logMessage("INFO",d_name);
					CURRENT_SECTION.hasDirs = 1;
					i=scanDirectory(path, files, i);
				} else {
					char path[PATH_MAX];
					snprintf (path, PATH_MAX, "%s%s", directory, d_name);
					files[i]=malloc(sizeof(path)+1);
					//				logMessage("INFO",path);
					strcpy(files[i],path);
					i++;
				}
			}
			free (d_name);
			free(result[j]);
			j++;
		}
		free(result);
		return i;
	}

	int recursivelyScanDirectory (char *directory, char* files[], int i) {
		DIR * d;
		d = opendir (directory);
		if (d==NULL) {
			return 0;
		}
		while (1) {
			struct dirent *entry;
			char * d_name;
			entry = readdir (d);
			if (!entry) {
				break;
			}
			d_name = entry->d_name;
			if (entry->d_type & DT_DIR) {
				if (strcmp(d_name, mediaFolder) != 0 && strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
					char path[PATH_MAX];
					char * e = strrchr(d_name, '/');
					if (e==NULL) {
						strcat(d_name, "/");
					}
					snprintf (path, PATH_MAX, "%s%s", directory, d_name);
					i=recursivelyScanDirectory(path, files, i);
				}
			} else {
				char path[PATH_MAX];
				snprintf (path, PATH_MAX, "%s%s", directory, d_name);
				files[i]=strdup(path);
				i++;
			}
		}
		free(d);
		return i;
	}

	int findDirectoriesInDirectory (char *directory, char* files[], int i) {
		DIR * d;
		d = opendir (directory);
		if (d==NULL) {
			return 0;
		}
		while (1) {
			struct dirent *entry;
			char * d_name;
			entry = readdir (d);
			if (!entry) {
				break;
			}
			d_name = entry->d_name;
			if (entry->d_type & DT_DIR) {
				if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
					char path[PATH_MAX];
					snprintf (path, PATH_MAX, "%s%s", directory, d_name);
					files[i]=malloc(sizeof(path));
					strcpy(files[i],path);
					i++;
				}
			}
		}
		free(d);
		return i;
	}


	int compareFavorites(const void *f1, const void *f2)
	{
		struct Favorite *e1 = (struct Favorite *)f1;
		struct Favorite *e2 = (struct Favorite *)f2;
		char temp1[300]="";
		char temp2[300]="";
		if (strlen(e1->alias)>1) {
			strcpy(temp1,e1->alias);
		} else {
			strcpy(temp1,e1->name);
			stripGameName(temp1);
		}
		if (strlen(e2->alias)>1) {
			strcpy(temp2,e2->alias);
		} else {
			strcpy(temp2,e2->name);
			stripGameName(temp2);
		}
		for(int i=0;temp1[i]; i++) {
			temp1[i] = tolower(temp1[i]);
		}
		for(int i=0;temp2[i]; i++) {
			temp2[i] = tolower(temp2[i]);
		}
		return strcmp(temp1, temp2);
	}

	void fillUpStolenGMenuFile(struct StolenGMenuFile* stolenFile, char* fileName) {
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		fp = fopen(fileName, "r");
		int paramsFlag = 0;
		while ((read = getline(&line, &len, fp)) != -1) {
			char *ptr;
			if(strstr(line,"title")!=NULL) {
				ptr = strtok(line, "=");
				ptr = strtok(NULL, "=");
				strcpy(stolenFile->title,ptr);
				logMessage("INFO", "STOLEN FILE title");
				if (stolenFile->title[strlen(stolenFile->title)-1]=='\n') {
					logMessage("INFO", "HAD ENTER");
					stolenFile->title[strlen(stolenFile->title)-1]='\0';
				}
				logMessage("INFO", stolenFile->title);
			} else if(strstr(line,"exec")!=NULL) {
				ptr = strtok(line, "=");
				ptr = strtok(NULL, "=");
				strcpy(stolenFile->exec,ptr);
				logMessage("INFO", "STOLEN FILE exec");
				if (stolenFile->exec[strlen(stolenFile->exec)-1]=='\n') {
					logMessage("INFO", "HAD ENTER");
					stolenFile->exec[strlen(stolenFile->exec)-1]='\0';
				} else {
					stolenFile->exec[strlen(stolenFile->exec)]='\0';
				}
				logMessage("INFO", stolenFile->exec);
			} else if(strstr(line,"params")!=NULL) {
				ptr = strtok(line, "=");
				ptr = strtok(NULL, "=");
				strcpy(stolenFile->params,ptr);
				logMessage("INFO", "STOLEN FILE params");
				if (stolenFile->params[strlen(stolenFile->params)-1]=='\n') {
					logMessage("INFO", "HAD ENTER");
					stolenFile->params[strlen(stolenFile->params)-1]='\0';
				}
				logMessage("INFO", stolenFile->params);
				paramsFlag=1;
			} else if(strstr(line,"consoleapp")!=NULL) {
				ptr = strtok(line, "=");
				ptr = strtok(NULL, "=");
				logMessage("INFO", "STOLEN FILE isConsoleApp");
				stolenFile->isConsoleApp=strcmp("false",ptr);
				logMessage("INFO", (stolenFile->isConsoleApp==0?"false":"true"));
				break;
			}
		}
		if (paramsFlag==0) {
			strcpy(stolenFile->params,"\0" );
		}
		fclose(fp);
		if(line) {
			free(line);
		}
	}

	int theSectionHasGames(struct MenuSection *section) {
		section->hidden=1;
		int dirCounter=0;
		char *dirs[10];
		char* ptr;
		char dirsCopy[1000];
		strcpy(dirsCopy,section->filesDirectories);
		ptr = strtok(dirsCopy, ",");
		char *files[MAX_GAMES_IN_SECTION];
		int value = 0;
		while (ptr!=NULL) {
			dirs[dirCounter]=malloc(strlen(ptr)+1);
			strcpy(dirs[dirCounter],ptr);
			ptr = strtok(NULL, ",");
			dirCounter++;
		}
		for(int k=0;k<dirCounter;k++) {
			int n = recursivelyScanDirectory(dirs[k], files, 0);
			for (int i=0;i<n;i++){
				char *ext = getExtension(files[i]);
				if (ext&&strcmp((files[i]),"..")!=0 &&
						strcmp((files[i]),".")!=0 &&
						strcmp(ext,".png")!=0&&
						isExtensionValid(ext,section->fileExtensions)) {
					value = 1;
					break;
				}
			}
			for (int i=0;i<n;i++){
				free(files[i]);
			}
			if (value==1) {
				section->hidden=0;
				break;
			}
		}
		for (int i=0;i<dirCounter;i++){
			free (dirs[i]);
		}
		return !value;
	}

	char * readline(FILE *fp, char *buffer)
	{
		int ch;
		int i = 0;
		size_t buff_len = 0;

		buffer = malloc(buff_len + 1);
		if (!buffer) return NULL;  // Out of memory

		while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		{
			buff_len++;
			void *tmp = realloc(buffer, buff_len + 1);
			if (tmp == NULL)
			{
				free(buffer);
				return NULL; // Out of memory
			}
			buffer = tmp;

			buffer[i] = (char) ch;
			i++;
		}
		buffer[i] = '\0';

		// Detect end
		if (ch == EOF && (i == 0 || ferror(fp)))
		{
			free(buffer);
			return NULL;
		}
		return buffer;
	}

	void loadGameList(int refresh) {
		logMessage("INFO",CURRENT_SECTION.sectionName);
		int loadedFiles=0;
		logMessage("INFO","Should we skip this?");
		FILE *fp=NULL;
		if (CURRENT_SECTION.initialized==0||refresh) {
			logMessage("INFO","No, loading game list");
			CURRENT_SECTION.initialized=1;
			//We don't need to reload the alias file if just refreshing
			if (!refresh) {
				loadAliasList(currentSectionNumber);
			}
			CURRENT_SECTION.totalPages=0;
			CURRENT_SECTION.gameCount=0;
			char *files[MAX_GAMES_IN_SECTION];
			int game = -1;
			int dirCounter=0;
			char *dirs[10];
			char *ptr=NULL;

			char sectionCacheName[PATH_MAX];

			if (refresh) {
				cleanListForSection(&CURRENT_SECTION);
				logMessage("INFO","Cleaned section list");
			}
			if (useCache==1) {
				if (refresh) {
					remove(sectionCacheName);
				}
				snprintf(sectionCacheName,sizeof(sectionCacheName),"%s/.simplemenu/tmp/%s.tmp",getenv("HOME"),CURRENT_SECTION.sectionName);
				fp = fopen(sectionCacheName,"r");
				if (fp!=NULL) {
					logMessage("INFO","Using cache file");
					char currentline[2000];
					while (fgets(currentline, sizeof(currentline), fp) != NULL) {
						int size = strlen(currentline)+1;

						struct Rom *rom;
						rom=malloc(sizeof(struct Rom));

						char *ptr = strtok(currentline,";");
						size = strlen(ptr)+1;
						rom->alias=malloc(size);
						if (strcmp(ptr,"(null)")==0) {
							free(rom->alias);
							rom->alias = "\0";
						} else {
							memcpy(rom->alias,ptr,size);
						}

						ptr = strtok(NULL,";");
						size = strlen(ptr)+1;
						rom->directory=strdup(ptr);

						ptr = strtok(NULL,";");
						size = strlen(ptr)+1;
						rom->name=strdup(ptr);
						rom->name[size-2] = '\0';
						if (game==ITEMS_PER_PAGE) {
							CURRENT_SECTION.totalPages++;
							game = 0;
						}
						InsertAtTail(rom);
						game++;
						CURRENT_SECTION.gameCount++;
					}
					logMessage("INFO","Finished reading cache");
					CURRENT_SECTION.tail=GetNthNode(CURRENT_SECTION.gameCount-1);
					scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
					if (fp!=NULL) {
						fclose(fp);
					}
					return;
				}
				if (fp!=NULL) {
					fclose(fp);
				}
			}
			char *filesDirectoriesCopy = strdup(CURRENT_SECTION.filesDirectories);
			ptr = strtok(filesDirectoriesCopy, ",");
			loading=1;
			while (ptr!=NULL) {
				dirs[dirCounter]=strdup(ptr);
				ptr = strtok(NULL, ",");
				dirCounter++;
			}
			free(filesDirectoriesCopy);
			for(int k=0;k<dirCounter;k++) {
				int n = 0;
				logMessage("INFO","Scanning directory");
				n = scanDirectory(dirs[k], files, 0);
				logMessage("INFO","Processing files");
				int realItemCount = n;
				for (int i=0;i<n;i++){
					char *ext = getExtension(files[i]);
					if (ext&&strcmp((files[i]),"..")!=0 &&
							strcmp((files[i]),".")!=0 &&
							isExtensionValid(ext,CURRENT_SECTION.fileExtensions)){
						//it's an opk
						if(strcmp(ext,".opk")==0) {
							struct OPKDesktopFile desktopFiles[10];
							int desktopFilesCount=getOPK(files[i], desktopFiles);
							int desktopCounter=0;
							while(desktopCounter<desktopFilesCount) {
								if(strstr(desktopFiles[desktopCounter].category,CURRENT_SECTION.category)==NULL&&strcmp(CURRENT_SECTION.category,"all")!=0) {
									break;
								} else {
#ifdef TARGET_RFW
									while(strstr(desktopFiles[desktopCounter].name,"gcw0")!=NULL) {
										logMessage("WARN", "Non-RetroFW desktop file found");
										desktopCounter++;
									}
#endif
									realItemCount++;
									int size = strlen(desktopFiles[desktopCounter].parentOPK)+strlen("-m|")+strlen(desktopFiles[desktopCounter].name)+2;// " -m "
									int aliasSize = strlen(desktopFiles[desktopCounter].displayName)+1;
									struct Rom *rom;
									rom=malloc(sizeof(struct Rom));
									rom->name=malloc(size);
									rom->alias=malloc(aliasSize);
									int directorySize = strlen(dirs[k])+1;
									rom->directory=malloc(directorySize);
									strcpy(rom->directory,dirs[k]);

									if (game==ITEMS_PER_PAGE) {
										if(desktopCounter!=realItemCount) {
											CURRENT_SECTION.totalPages++;
											game = 0;
										}
									}
#ifdef TARGET_RFW
									if(strstr(desktopFiles[desktopCounter].name,"default")==NULL) {
										strcpy(rom->name,"-m|");
										strcat(rom->name,desktopFiles[desktopCounter].name);
										strcat(rom->name,"|");
									} else {
										strcpy(rom->name,desktopFiles[desktopCounter].parentOPK);
										strcpy(rom->alias,desktopFiles[desktopCounter].displayName);
									}
#else
									strcpy(rom->name,"-m|");
									strcat(rom->name,desktopFiles[desktopCounter].name);
									strcat(rom->name,"|");
									strcat(rom->name,desktopFiles[desktopCounter].parentOPK);
									strcpy(rom->alias,desktopFiles[desktopCounter].displayName);
#endif
									InsertAtTail(rom);
									game++;
									CURRENT_SECTION.gameCount++;
								}
								desktopCounter++;
							}
							loadedFiles++;
						}
						//it's not an opk
						else {
							int size = 2000;
							struct Rom *rom;
							rom=malloc(sizeof(struct Rom));
							rom->name=malloc(size);

							int directorySize = strlen(dirs[k])+1;
							rom->directory=malloc(directorySize);
							strcpy(rom->directory,dirs[k]);
							rom->isConsoleApp=0;
							//it's a custom link
							if(strcmp(ext,".fgl")==0) {
								struct StolenGMenuFile stolenFile;
								logMessage("INFO", "STOLEN FILE!!!");
								fillUpStolenGMenuFile(&stolenFile, files[i]);
								strcpy(rom->name,stolenFile.exec);
								if(stolenFile.params[0]!='\0') {
									strcat(rom->name," ");
									strcat(rom->name,stolenFile.params);
								}
								logMessage("INFO", "FINAL NAME");
								logMessage("INFO", rom->name);
								rom->alias=malloc(strlen(stolenFile.title)+1);
								strcpy(rom->alias, stolenFile.title);
								rom->isConsoleApp=stolenFile.isConsoleApp;
							} else {
								//it's a rom
								strcpy(rom->name,files[i]);
								if(strlen(CURRENT_SECTION.aliasFileName)>2) {
									char *temp=getAlias(rom->name);
									rom->alias=malloc(strlen(temp)+1);
									strcpy(rom->alias, temp);
									free(temp);
								} else {
									rom->alias=NULL;
								}
							}
							CURRENT_SECTION.gameCount++;
							if (game==ITEMS_PER_PAGE) {
								if(i!=realItemCount) {
									CURRENT_SECTION.totalPages++;
									game = 0;
								}
							}
							InsertAtTail(rom);
							loadedFiles++;
							game++;
						}
					}
				}
				logMessage("INFO","All files loaded");
				for (int i=0;i<n;i++){
					free(files[i]);
				}
				if (loadedFiles==0&&k==(dirCounter-1)) {
					CURRENT_SECTION.hidden=1;
					loading=0;
					if (fp!=NULL) {
						fclose(fp);
					}
					return;
				}
			}
			for (int i=0;i<dirCounter;i++){
				free (dirs[i]);
			}
			logMessage("INFO","The list needs to be sorted");
			CURRENT_SECTION.head = mergeSort(CURRENT_SECTION.head);

			if(useCache==1) {
				fp = fopen(sectionCacheName,"w");
				for (int i=0;i<CURRENT_SECTION.gameCount;i++) {
					struct Rom *rom;
					rom = GetNthNode(i)->data;
					fprintf(fp,"%s;%s;%s\n",rom->alias, rom->directory, rom->name);
				}
			}
			CURRENT_SECTION.tail=GetNthNode(CURRENT_SECTION.gameCount-1);
			scrollToGame(CURRENT_SECTION.realCurrentGameNumber);
			if (fp!=NULL) {
				fclose(fp);
			}
		}
		loading=0;
	}

	int countGamesInPage() {
		int gamesCounter=0;
		struct Node *node;
		int number = CURRENT_GAME_NUMBER;
		while(number%ITEMS_PER_PAGE!=0) {
			number--;
		}
		node = CURRENT_SECTION.currentGameNode;
		for (int i=number;i<number+ITEMS_PER_PAGE;i++) {
			if (node!=NULL) {
				if (node->data->name!=NULL && strlen(node->data->name)>1) {
					gamesCounter++;
				}
				if (node->next==NULL) {
					break;
				}
				node=node->next;
			}
		}
		return gamesCounter;
	}

	void selectRandomGame() {
		int game = rand() % CURRENT_SECTION.gameCount;
		scrollToGame(game);
	}

	void determineStartingScreen(int sectionCount) {
		if (sectionCount==-1) {
			generateError("SECTIONS FILE NOT FOUND-SHUTTING DOWN",1);
			return;
		}
		if(sectionCount==0||currentSectionNumber==favoritesSectionNumber) {
			logMessage("INFO","No sections found or favorites was selected");
			favoritesSectionSelected=1;
			loadFavoritesSectionGameList();
			logMessage("INFO","Favorites loaded");
			if (CURRENT_SECTION.backgroundSurface == NULL) {
				logMessage("INFO","Loading system background");
				CURRENT_SECTION.backgroundSurface = IMG_Load(CURRENT_SECTION.background);
				resizeSectionBackground(&CURRENT_SECTION);
				CURRENT_SECTION.systemPictureSurface = IMG_Load(CURRENT_SECTION.systemPicture);
				resizeSectionSystemPicture(&CURRENT_SECTION);
			}
			logMessage("INFO","Section background loaded and resized");
			int gamesInSection=CURRENT_SECTION.gameCount;
			int pages = gamesInSection / ITEMS_PER_PAGE;
			if (gamesInSection%ITEMS_PER_PAGE==0) {
				pages--;
			}
			logMessage("INFO","Pages set");
			CURRENT_SECTION.totalPages=pages;
			if (favoritesSize==0&&sectionCount>0) {
				logMessage("INFO","Favorites was selected but no favorites were found");
				favoritesSectionSelected=0;
				currentSectionNumber=0;
				logMessage("INFO","Trying to determine starting section again");
				determineStartingScreen(sectionCount);
			}
		} else {
			logMessage("INFO","determineStartingScreen - Loading game list");
			loadGameList(0);
			int pages = CURRENT_SECTION.gameCount / ITEMS_PER_PAGE;
			if (pages>0&&CURRENT_SECTION.gameCount%ITEMS_PER_PAGE==0) {
				pages--;
			}
			CURRENT_SECTION.totalPages=pages;
		}
		logMessage("INFO","Found starting screen");
	}

	void deleteGame(struct Rom *rom) {
		char command [300];
		snprintf(command,sizeof(command),"rm \"%s\";",rom->name);
		int ret = system(command);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
		char *pictureWithFullPath=malloc(600);
		char *tempGameName;
		char *tempGameName1;
		strcpy(pictureWithFullPath, rom->directory);
		tempGameName=getGameName(rom->name);
		strcat(pictureWithFullPath,mediaFolder);
		strcat(pictureWithFullPath,"/");
		tempGameName1=getNameWithoutExtension(tempGameName);
		strcat(pictureWithFullPath,tempGameName);
		strcat(pictureWithFullPath,".png");
		snprintf(command,sizeof(command),"rm \"%s\";",pictureWithFullPath);
		free(tempGameName);
		free(tempGameName1);
		free(pictureWithFullPath);
		ret = system(command);
		if (ret==-1) {
			generateError("FATAL ERROR", 1);
		}
	}

	int is43 () {
		return SCREEN_RATIO<=1.34&&SCREEN_RATIO>=1.33;
	}
